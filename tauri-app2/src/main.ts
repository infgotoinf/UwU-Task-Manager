import { invoke } from "@tauri-apps/api/core";

let table: HTMLElement | null = document.querySelector(".table");
let selectedPIDs = new Set<string>();
let allPIDs = new Set<string>();
let currentData: Array<string> = [];
let sortField: "name" | "pid" | "mem_use" | "cpu_use" | "gpu_use";

async function updateProcesses() {
  let printed_process_info: Array<string> = await invoke("print_process_info");
  currentData = printed_process_info;
  renderTable();
}

function renderTable() {
  if (!table) return;
  allPIDs = new Set<string>();

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
      <td class="mem_use">${proc.mem_use}Mb</td>
      <td class="cpu_use">${proc.cpu_use}%</td>
      <td class="gpu_use">${proc.gpu_use}%</td>`;
    allPIDs.add(proc.pid);
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

  const btn = document.createElement("button");
  btn.textContent = "Reset";
  btn.classList.add("first-row-btn");
  btn.addEventListener("click", () => {
    selectedPIDs = new Set<string>();
  });
  firstRow.appendChild(btn);

  const btn2 = document.createElement("button");
  btn2.textContent = "Select All";
  btn2.classList.add("first-row-btn");
  btn2.addEventListener("click", () => {
    selectedPIDs = allPIDs;
  });
  firstRow.appendChild(btn2);


  let Field: "name" | "pid" | "mem_use" | "cpu_use" | "gpu_use";
  let field_pid: typeof Field = "pid";
  let field_name: typeof Field = "name";
  let field_mem: typeof Field = "mem_use";
  let field_cpu: typeof Field = "cpu_use";
  let field_gpu: typeof Field = "gpu_use";
  const fields = [
    { field: field_pid, label: "PID" },
    { field: field_name, label: "Name" },
    { field: field_mem, label: "Memory" },
    { field: field_cpu, label: "CPU" },
    { field: field_gpu, label: "GPU" },
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

  const btn3 = document.createElement("button");
  btn3.textContent = "Reset";
  btn3.classList.add("second-row-btn");
  btn3.addEventListener("click", () => {
    sortField = "gpu_use";
    renderTable();
  });
  secondRow.appendChild(btn3);

  table?.parentElement?.insertBefore(buttonContainer, table);
}

createSortButtons();

document.addEventListener('DOMContentLoaded', () => {
  setInterval(updateProcesses, 1000);
  updateProcesses();
});