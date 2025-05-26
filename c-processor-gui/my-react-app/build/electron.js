const { app, BrowserWindow, ipcMain } = require("electron");
const path = require("path");
const fs = require("fs");
const { spawn } = require("child_process");

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1400,
    height: 900,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      preload: path.join(__dirname, "preload.js"),
    },
  });

  const isDev = process.env.NODE_ENV === "development";

  if (isDev) {
    mainWindow.loadURL("http://localhost:3000");
    mainWindow.webContents.openDevTools();
  } else {
    mainWindow.loadFile(path.join(__dirname, "../build/index.html"));
  }
}

app.whenReady().then(createWindow);

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

app.on("activate", () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});

ipcMain.handle("create-file-and-run-exe", async (event, instruction) => {
  try {
    const appRoot = process.cwd();

    // Navigate up to find the project root (where bin/Debug/main.exe exists)
    let projectRoot = appRoot;
    let maxLevels = 5; // Prevent infinite loop

    while (maxLevels > 0) {
      const testExePath = path.join(projectRoot, "bin", "Debug", "main.exe");
      if (fs.existsSync(testExePath)) {
        console.log("Found project root at:", projectRoot);
        break;
      }
      const parentDir = path.dirname(projectRoot);
      if (parentDir === projectRoot) {
        // Reached filesystem root
        break;
      }
      projectRoot = parentDir;
      maxLevels--;
    }

    console.log("App root:", appRoot);
    console.log("Project root:", projectRoot);

    const dataFolder = path.join(projectRoot, "data");
    if (!fs.existsSync(dataFolder)) {
      fs.mkdirSync(dataFolder, { recursive: true });
      console.log("Created data folder:", dataFolder);
    }

    const filePath = path.join(dataFolder, "test.txt");
    const fileContent = instruction || "No instruction provided";
    fs.writeFileSync(filePath, fileContent, "utf8");
    console.log("Created file:", filePath);

    // Clean up old log files before execution
    const registersLogPath = path.join(dataFolder, "registers_log.txt");
    const memoryLogPath = path.join(dataFolder, "memory_log.txt");

    if (fs.existsSync(registersLogPath)) {
      fs.unlinkSync(registersLogPath);
      console.log("Removed old registers log");
    }
    if (fs.existsSync(memoryLogPath)) {
      fs.unlinkSync(memoryLogPath);
      console.log("Removed old memory log");
    }

    // Try multiple possible locations for main.exe
    const possibleExePaths = [
      path.join(projectRoot, "bin", "Debug", "main.exe"),
      path.join(projectRoot, "bin", "main.exe"),
    ];

    let exePath = null;
    let workingDir = null;

    for (const possiblePath of possibleExePaths) {
      console.log("Checking for executable at:", possiblePath);
      if (fs.existsSync(possiblePath)) {
        exePath = possiblePath;
        workingDir = projectRoot; // Set working directory to project root
        console.log("Found executable at:", exePath);
        console.log("Working directory:", workingDir);
        break;
      }
    }

    if (!exePath) {
      const errorMsg = `main.exe not found. Searched in:\n${possibleExePaths.join(
        "\n"
      )}`;
      console.error(errorMsg);
      throw new Error(errorMsg);
    }

    return new Promise((resolve, reject) => {
      executeExe(exePath, workingDir, resolve, reject, filePath, dataFolder);
    });
  } catch (error) {
    console.error("Error in create-file-and-run-exe:", error);
    throw error;
  }
});

function executeExe(
  exePath,
  workingDir,
  resolve,
  reject,
  filePath,
  dataFolder
) {
  console.log("Executing:", exePath);
  console.log("Working directory:", workingDir);
  console.log("Input file:", filePath);
  console.log("Data directory:", dataFolder);

  // Convert paths to relative paths from working directory
  const relativeInputFile = path.relative(workingDir, filePath);
  const relativeDataDir = path.relative(workingDir, dataFolder);

  console.log("Relative input file:", relativeInputFile);
  console.log("Relative data dir:", relativeDataDir);

  const child = spawn(exePath, [], {
    cwd: workingDir,
    env: {
      ...process.env,
      INPUT_FILE: filePath,
      DATA_DIR: dataFolder,
      // Also provide relative paths
      INPUT_FILE_REL: relativeInputFile,
      DATA_DIR_REL: relativeDataDir,
    },
    stdio: ["pipe", "pipe", "pipe"], // Ensure all stdio streams are piped
    shell: false, // Don't use shell, execute directly
  });

  let stdout = "";
  let stderr = "";

  child.stdout.on("data", (data) => {
    const output = data.toString();
    stdout += output;
    console.log("STDOUT:", output);
  });

  child.stderr.on("data", (data) => {
    const error = data.toString();
    stderr += error;
    console.log("STDERR:", error);
  });

  child.on("close", (code) => {
    console.log(`Executable exited with code ${code}`);

    // Add a small delay to ensure log files are written
    setTimeout(() => {
      let registersData = {};
      let memoryData = {};

      try {
        const registersLogPath = path.join(dataFolder, "registers_log.txt");
        console.log("Looking for registers log at:", registersLogPath);

        if (fs.existsSync(registersLogPath)) {
          console.log("Found registers log file");
          const registersContent = fs.readFileSync(registersLogPath, "utf8");
          console.log("Registers log content:", registersContent);
          registersData = parseRegistersLog(registersContent);
          console.log("Parsed registers data:", registersData);
        } else {
          console.log("Registers log file not found");
        }

        const memoryLogPath = path.join(dataFolder, "memory_log.txt");
        console.log("Looking for memory log at:", memoryLogPath);

        if (fs.existsSync(memoryLogPath)) {
          console.log("Found memory log file");
          const memoryContent = fs.readFileSync(memoryLogPath, "utf8");
          console.log("Memory log content:", memoryContent);
          memoryData = parseMemoryLog(memoryContent);
          console.log("Parsed memory data:", memoryData);
        } else {
          console.log("Memory log file not found");
        }
      } catch (logError) {
        console.error("Error reading log files:", logError);
      }

      const result = {
        success: code === 0,
        exitCode: code,
        message:
          code === 0
            ? "File created and executable ran successfully"
            : `Executable exited with code ${code}`,
        filePath: filePath,
        exePath: exePath,
        workingDir: workingDir,
        stdout: stdout,
        stderr: stderr,
        registersData: registersData,
        memoryData: memoryData,
      };

      console.log("Final result:", result);
      resolve(result);
    }, 100); // 100ms delay to ensure file system operations complete
  });

  child.on("error", (err) => {
    console.error("Failed to start process:", err);
    reject(new Error(`Failed to start executable: ${err.message}`));
  });

  // Set a timeout to prevent hanging
  const timeout = setTimeout(() => {
    console.log("Executable timed out, killing process");
    child.kill("SIGTERM");
    reject(new Error("Executable execution timed out"));
  }, 30000); // 30 second timeout

  child.on("close", () => {
    clearTimeout(timeout);
  });
}

function parseRegistersLog(content) {
  const registers = {};
  const lines = content.split(/\r?\n/); // Handle both Unix and Windows line endings

  console.log("Parsing registers, total lines:", lines.length);

  for (const line of lines) {
    const trimmedLine = line.trim();
    if (trimmedLine) {
      console.log("Processing register line:", trimmedLine);

      // Try multiple patterns to match register entries
      let match = trimmedLine.match(/^([A-Z0-9]+):\s*(.+)$/);
      if (!match) {
        // Try without colon
        match = trimmedLine.match(/^([A-Z0-9]+)\s+(.+)$/);
      }
      if (!match) {
        // Try with equals
        match = trimmedLine.match(/^([A-Z0-9]+)\s*=\s*(.+)$/);
      }

      if (match) {
        const registerName = match[1];
        const value = match[2].trim();

        // Convert hex values to decimal if needed
        let parsedValue = value;
        if (value.startsWith("0x") || value.startsWith("0X")) {
          parsedValue = parseInt(value, 16);
        } else if (!isNaN(value)) {
          parsedValue = parseInt(value);
        }

        registers[registerName] = parsedValue;
        console.log(`Parsed register: ${registerName} = ${parsedValue}`);
      } else {
        console.log("Could not parse register line:", trimmedLine);
      }
    }
  }

  console.log("Final parsed registers:", registers);
  return registers;
}

function parseMemoryLog(content) {
  const memory = {};
  const lines = content.split(/\r?\n/); // Handle both Unix and Windows line endings

  console.log("Parsing memory, total lines:", lines.length);

  for (const line of lines) {
    const trimmedLine = line.trim();
    if (trimmedLine) {
      console.log("Processing memory line:", trimmedLine);

      // Try multiple patterns to match memory entries
      let match = trimmedLine.match(/^(?:Address\s+)?(\d+):\s*(.+)$/);
      if (!match) {
        // Try without "Address" prefix
        match = trimmedLine.match(/^(\d+)\s+(.+)$/);
      }
      if (!match) {
        // Try with equals
        match = trimmedLine.match(/^(?:Address\s+)?(\d+)\s*=\s*(.+)$/);
      }

      if (match) {
        const address = parseInt(match[1]);
        const value = match[2].trim();

        // Convert hex values to decimal if needed
        let parsedValue = value;
        if (value.startsWith("0x") || value.startsWith("0X")) {
          parsedValue = parseInt(value, 16);
        } else if (!isNaN(value)) {
          parsedValue = parseInt(value);
        }

        memory[address] = parsedValue;
        console.log(`Parsed memory: ${address} = ${parsedValue}`);
      } else {
        console.log("Could not parse memory line:", trimmedLine);
      }
    }
  }

  console.log("Final parsed memory:", memory);
  return memory;
}
