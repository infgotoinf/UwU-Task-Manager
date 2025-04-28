import { invoke } from "@tauri-apps/api/core";

let table: HTMLElement | null = document.querySelector(".table");
const selectedPIDs = new Set<string>(); 

async function greet() {
  let da: Array<string> = await invoke("my_custom_command2");

  const oldRows = document.querySelectorAll(".process-row");
  oldRows.forEach(row => row.remove());

  for (let i = 0; i < da.length; i += 5) {
    const name    = da[i];
    const pid     = da[i + 1];
    const mem_use = da[i + 2];
    const cpu_use = da[i + 3];
    const gpu_use = da[i + 4];

    const row = document.createElement("tr");
    row.classList.add("process-row");

    const checkbox = document.createElement("input");
    checkbox.type = "checkbox";
    checkbox.checked = selectedPIDs.has(pid);
    checkbox.addEventListener("change", () => {
      if (checkbox.checked) {
        selectedPIDs.add(pid);
      } else {
        selectedPIDs.delete(pid);
      }
    });

    row.innerHTML = `
    <td class="pid">${pid}</td>
    <td class="name">${name}</td>
    <td class="mem_use">${mem_use}</td>
    <td class="cpu_use">${cpu_use}</td>
    <td class="gpu_use">${gpu_use}</td>`;
    row.appendChild(checkbox);
    table?.appendChild(row);
  }
}

document.addEventListener('DOMContentLoaded', () => {
  setInterval(greet, 1000);
  greet();
});