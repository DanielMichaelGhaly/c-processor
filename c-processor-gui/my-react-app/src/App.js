import React, { useState } from "react";

function App() {
  const [instruction, setInstruction] = useState("");

  // Initialize registers (R0-R31 + PC)
  const [registers, setRegisters] = useState(() => {
    const regs = {};
    for (let i = 0; i <= 31; i++) {
      regs[`R${i}`] = 0;
    }
    regs["PC"] = 0;
    return regs;
  });

  // Initialize memory (1-2048)
  const [memory, setMemory] = useState(() => {
    const mem = {};
    for (let i = 1; i <= 2048; i++) {
      mem[i] = 0;
    }
    return mem;
  });

  const handleInstructionChange = (e) => {
    setInstruction(e.target.value);
  };

  const handleStart = async () => {
    try {
      // Check if we're running in Electron
      if (window.electronAPI) {
        // Electron environment - can create files and run executables
        const result = await window.electronAPI.createFileAndRunExe(
          instruction
        );

        console.log("Success:", result.message);
        console.log("File created at:", result.filePath);
        if (result.stdout) {
          console.log("Program output:", result.stdout);
        }
        if (result.stderr) {
          console.log("Program errors:", result.stderr);
        }

        // Update registers and memory from log files
        if (
          result.registersData &&
          Object.keys(result.registersData).length > 0
        ) {
          setRegisters((prevRegs) => {
            const newRegs = { ...prevRegs };
            // Update with data from registers_log.txt
            Object.entries(result.registersData).forEach(([reg, value]) => {
              newRegs[reg] = value;
            });
            return newRegs;
          });
          console.log("Registers updated from log file");
        }

        if (result.memoryData && Object.keys(result.memoryData).length > 0) {
          setMemory((prevMem) => {
            const newMem = { ...prevMem };
            // Update with data from memory_log.txt
            Object.entries(result.memoryData).forEach(([addr, value]) => {
              newMem[parseInt(addr)] = value;
            });
            return newMem;
          });
          console.log("Memory updated from log file");
        }

        alert(
          `Success! File created and main.exe executed.\nFile: ${
            result.filePath
          }\nOutput: ${result.stdout || "No output"}\nRegisters updated: ${
            Object.keys(result.registersData || {}).length
          }\nMemory updated: ${Object.keys(result.memoryData || {}).length}`
        );
      } else {
        // Browser environment - fallback to download
        const fileContent = instruction || "No instruction provided";
        const blob = new Blob([fileContent], { type: "text/plain" });
        const url = URL.createObjectURL(blob);

        const link = document.createElement("a");
        link.href = url;
        link.download = "test.txt";
        document.body.appendChild(link);
        link.click();
        document.body.removeChild(link);

        URL.revokeObjectURL(url);

        alert(
          "Running in browser mode. File downloaded as test.txt. Run as desktop app for full functionality."
        );
      }
    } catch (error) {
      console.error("Error:", error);
      alert("Error: " + error.message);
    }
  };

  const renderRegistersTable = () => {
    const registerEntries = Object.entries(registers);

    return (
      <div className="w-1/2 pr-4">
        <h3 className="text-lg font-semibold mb-3 text-gray-800">Registers</h3>
        <div className="bg-white border border-gray-300 rounded-lg shadow-sm max-h-96 overflow-auto">
          <table className="w-full text-sm">
            <thead className="bg-gray-50 sticky top-0">
              <tr>
                <th className="px-4 py-2 text-left font-medium text-gray-700 border-b">
                  Register
                </th>
                <th className="px-4 py-2 text-left font-medium text-gray-700 border-b">
                  Value
                </th>
              </tr>
            </thead>
            <tbody>
              {registerEntries.map(([reg, value]) => (
                <tr key={reg} className="hover:bg-gray-50">
                  <td className="px-4 py-2 font-mono text-blue-600 border-b border-gray-100">
                    {reg}
                  </td>
                  <td className="px-4 py-2 font-mono border-b border-gray-100">
                    {value}
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </div>
    );
  };

  const renderMemoryTable = () => {
    const memoryEntries = Object.entries(memory);

    return (
      <div className="w-1/2 pl-4">
        <h3 className="text-lg font-semibold mb-3 text-gray-800">Memory</h3>
        <div className="bg-white border border-gray-300 rounded-lg shadow-sm max-h-96 overflow-auto">
          <table className="w-full text-sm">
            <thead className="bg-gray-50 sticky top-0">
              <tr>
                <th className="px-4 py-2 text-left font-medium text-gray-700 border-b">
                  Address
                </th>
                <th className="px-4 py-2 text-left font-medium text-gray-700 border-b">
                  Value
                </th>
              </tr>
            </thead>
            <tbody>
              {memoryEntries.map(([address, value]) => (
                <tr key={address} className="hover:bg-gray-50">
                  <td className="px-4 py-2 font-mono text-green-600 border-b border-gray-100">
                    {address}
                  </td>
                  <td className="px-4 py-2 font-mono border-b border-gray-100">
                    {value}
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </div>
    );
  };

  return (
    <div className="min-h-screen bg-gray-100 p-6">
      {/* Title */}
      <div className="text-center mb-8">
        <h1 className="text-4xl font-bold text-gray-800 mb-2">C-Processor</h1>
        <div className="w-24 h-1 bg-blue-500 mx-auto"></div>
      </div>

      {/* Instruction Input */}
      <div className="max-w-4xl mx-auto mb-8">
        <div className="bg-white rounded-lg shadow-sm border border-gray-300 p-6">
          <label
            htmlFor="instruction"
            className="block text-lg font-medium text-gray-700 mb-3"
          >
            Enter Instructions:
          </label>
          <div className="flex gap-4">
            <textarea
              id="instruction"
              value={instruction}
              rows={1024}
              onChange={handleInstructionChange}
              className="h-48 flex-1 px-4 py-2 border border-gray-300 rounded-md focus:ring-2 focus:ring-blue-500 focus:border-transparent font-mono text-lg"
              placeholder="Enter instructions line by line..."
            />
            <button
              onClick={handleStart}
              className="px-6 py-2 bg-green-600 text-white rounded-md hover:bg-green-700 focus:ring-2 focus:ring-green-500 focus:ring-offset-2 font-medium"
            >
              Start
            </button>
          </div>
        </div>
      </div>

      {/* Tables Container */}
      <div className="max-w-6xl mx-auto">
        <div className="flex gap-0">
          {renderRegistersTable()}
          {renderMemoryTable()}
        </div>
      </div>
    </div>
  );
}

export default App;
