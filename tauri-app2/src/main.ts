import { invoke } from "@tauri-apps/api/core";

let table: HTMLElement | null = document.querySelector(".table");
const selectedPIDs = new Set<string>();
let currentData: Array<string> = [];
let sortField: string | null = null;

async function updateProcesses() {
  let printed_process_info: Array<string> = await invoke("print_process_info");
  currentData = printed_process_info;
  renderTable();
}

function renderTable() {
  if (!table) return;

  const oldRows = document.querySelectorAll(".process-row");
  oldRows.forEach(row => row.remove());

  let processes: Array<{ name: string; pid: string; mem_use: string; cpu_use: string; gpu_use: string; }> = [];
  for (let i = 0; i < currentData.length; i += 5) {
    processes.push({
      name: currentData[i],
      pid: currentData[i + 1],
      mem_use: currentData[i + 2],
      cpu_use: currentData[i + 3],
      gpu_use: currentData[i + 4],
    });
  }

  if (sortField) {
    processes.sort((a, b) => {
      if (sortField === "name") {
        return a.name.localeCompare(b.name);
      } else {
        return parseFloat(b[sortField]) - parseFloat(a[sortField]);
      }
    });
  }

  processes.forEach(proc => {
    const row = document.createElement("tr");
    row.classList.add("process-row");

    const checkbox = document.createElement("input");
    checkbox.type = "checkbox";
    checkbox.checked = selectedPIDs.has(proc.pid);
    checkbox.addEventListener("change", () => {
      if (checkbox.checked) {
        selectedPIDs.add(proc.pid);
      } else {
        selectedPIDs.delete(proc.pid);
      }
    });
    row.innerHTML = `
      <th></th>
      <th></th>
      <th></th>
      <th></th>
      <th></th>`;

    row.innerHTML = `
      <td class="pid">${proc.pid}</td>
      <td class="name">${proc.name}</td>
      <td class="mem_use">${proc.mem_use}</td>
      <td class="cpu_use">${proc.cpu_use}</td>
      <td class="gpu_use">${proc.gpu_use}</td>`;
    row.appendChild(checkbox);
    table?.appendChild(row);
  });
}


function createSortButtons() {
  const buttonContainer = document.createElement("div");
  const firstRow = document.createElement("div");
  firstRow.classList.add("first-row");
  const secondRow = document.createElement("div");
  secondRow.classList.add("second-row");


  const btn = document.createElement("button");
  btn.textContent = "Reset";
  btn.id = "reset";
  btn.classList.add("first-row-btn");
  btn.addEventListener("click", () => {
    sortField = null;
    renderTable();
  });
  firstRow.appendChild(btn);

  const killButton = document.createElement("button");
  killButton.textContent = "Kill All Selected";
  killButton.id = "kill_btn";
  killButton.classList.add("first-row-btn");
  killButton.addEventListener("click", async () => {
  for (const pid of selectedPIDs) {
    try {
      await invoke("kill_process", { pid: parseInt(pid) });
    } catch (error) {
      console.error(`Failed to kill process ${pid}:`, error);
    }
  }
  selectedPIDs.clear();
  updateProcesses();
  });
  firstRow.appendChild(killButton);
  buttonContainer.appendChild(firstRow);


  const fields = [
    { field: "pid", label: `PID` },
    { field: "name", label: "Name" },
    { field: "mem_use", label: "Memory" },
    { field: "cpu_use", label: "CPU" },
    { field: "gpu_use", label: "GPU" },
  ];

  fields.forEach(({ field, label }) => {
    const btn = document.createElement("button");
    btn.textContent = label;
    btn.id = `${field}`;
    btn.classList.add("second-row-btn");
    btn.addEventListener("click", () => {
      sortField = field;
      renderTable();
    });
    secondRow.appendChild(btn);
    buttonContainer.appendChild(secondRow);
  });

  table?.parentElement?.insertBefore(buttonContainer, table);
}

createSortButtons();

document.addEventListener('DOMContentLoaded', () => {
  setInterval(updateProcesses, 1000);
  updateProcesses();
});