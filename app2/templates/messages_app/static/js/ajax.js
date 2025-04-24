const get_data_url = '/get_data/';
const table = document.querySelector(".table");

function updateProcesses() {
    fetch(get_data_url)
        .then(response => response.json())
        .then(data => {
            const processes = data.processes;

            const oldRows = document.querySelectorAll(".process-row");
            oldRows.forEach(row => row.remove());

            for (const pid in processes) {
                const process = processes[pid];
                const row = document.createElement("div");
                row.classList.add("process-row");

                row.innerHTML = `
                    <div>${process.name}</div>
                    <div>${pid}</div>
                    <div>${process.memUse}Mb</div>
                    <div>${process.cpu || '—'}</div>
                `;

                table.appendChild(row);
            }
        })
        .catch(error => console.error("Ошибка при получении данных:", error));
}

document.addEventListener("DOMContentLoaded", () => {
    setInterval(updateProcesses, 1000);
    updateProcesses();
});