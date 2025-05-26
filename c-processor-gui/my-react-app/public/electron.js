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
    const project = path.dirname(appRoot);
    const projectRoot = path.dirname(project);

    const dataFolder = path.join(projectRoot, "data");
    if (!fs.existsSync(dataFolder)) {
      fs.mkdirSync(dataFolder, { recursive: true });
    }

    const filePath = path.join(dataFolder, "test.txt");
    const fileContent = instruction || "No instruction provided";
    fs.writeFileSync(filePath, fileContent, "utf8");

    const binFolder = path.join(projectRoot, "bin", "Debug");
    const exePath = path.join(binFolder, "main.exe");

    return new Promise((resolve, reject) => {
      if (!fs.existsSync(exePath)) {
        const fallbackExePath = path.join(projectRoot, "bin", "main.exe");
        if (!fs.existsSync(fallbackExePath)) {
          reject(
            new Error(`main.exe not found at: ${exePath} or ${fallbackExePath}`)
          );
          return;
        } else {
          executeExe(
            fallbackExePath,
            path.join(projectRoot, "bin"),
            resolve,
            reject,
            filePath,
            dataFolder
          );
          return;
        }
      }
      executeExe(exePath, binFolder, resolve, reject, filePath, dataFolder);
    });
  } catch (error) {
    console.error("Error:", error);
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
  console.log("Running executable...");
  const child = spawn(exePath, [], {
    cwd: workingDir,
    env: {
      ...process.env,
      INPUT_FILE: filePath,
      DATA_DIR: dataFolder,
    },
  });

  let stdout = "";
  let stderr = "";

  child.stdout.on("data", (data) => {
    stdout += data.toString();
  });

  child.stderr.on("data", (data) => {
    stderr += data.toString();
  });

  child.on("close", (code) => {
    console.log(`Executable exited with code ${code}`);
    let registersData = {};
    let memoryData = {};

    try {
      const registersLogPath = path.join(dataFolder, "registers_log.txt");
      if (fs.existsSync(registersLogPath)) {
        const registersContent = fs.readFileSync(registersLogPath, "utf8");
        registersData = parseRegistersLog(registersContent);
      }

      const memoryLogPath = path.join(dataFolder, "memory_log.txt");
      if (fs.existsSync(memoryLogPath)) {
        const memoryContent = fs.readFileSync(memoryLogPath, "utf8");
        memoryData = parseMemoryLog(memoryContent);
      }
    } catch (logError) {
      console.error("Error reading log files:", logError);
    }

    resolve({
      success: true,
      message: "File created and executable ran successfully",
      filePath: filePath,
      exePath: exePath,
      stdout: stdout,
      stderr: stderr,
      registersData: registersData,
      memoryData: memoryData,
    });
  });

  child.on("error", (err) => {
    console.error("Failed to start process:", err);
    reject(err);
  });
}

function parseRegistersLog(content) {
  const registers = {};
  const lines = content.split("\n");
  for (const line of lines) {
    const trimmedLine = line.trim();
    if (trimmedLine) {
      const match = trimmedLine.match(/^([A-Z0-9]+):\s*(.+)$/);
      if (match) {
        const registerName = match[1];
        const value = match[2];
        registers[registerName] = value;
      }
    }
  }
  return registers;
}

function parseMemoryLog(content) {
  const memory = {};
  const lines = content.split("\n");
  for (const line of lines) {
    const trimmedLine = line.trim();
    if (trimmedLine) {
      const match = trimmedLine.match(/^(?:Address\s+)?(\d+):\s*(.+)$/);
      if (match) {
        const address = parseInt(match[1]);
        const value = match[2];
        memory[address] = value;
      }
    }
  }
  return memory;
}
