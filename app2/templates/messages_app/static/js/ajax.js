const get_data_url = '/get_data/';
const container = document.getElementById('process-container');

const table = document.querySelector(".table");

function updateProcesses() {
    fetch(get_data_url)
        .then(response => response.json())
        .then(data => {
            const processes = data.processes;

            // Удаляем старые строки
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
                    <div>${process.CPUUse}</div>
                `;
                table.appendChild(row);
            }
        })
        .catch(error => console.error('Error:', error));
}

document.addEventListener('DOMContentLoaded', () => {
    setInterval(updateProcesses, 1000);
    updateProcesses();
});