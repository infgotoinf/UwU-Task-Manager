const get_data_url = '/get_data/';
const container = document.getElementById('process-container');

const table = document.querySelector(".table");

function updateProcesses() {
    fetch(get_data_url)
        .then(response => response.json())
        .then(data => {
            table.innerHTML = ""; 
            const processes = data.processes;
            for (const pid in processes) {
                const process = processes[pid];
                const row = document.createElement("div");
                row.classList.add("process-row");
                row.innerHTML = `
                    <div class="process">
                        <span class="name">Name: ${process.name}</span>
                        <span class="pid">PID: ${pid}</span>
                        <span class="memUse">MemUse: ${process.memUse}</span>
                    </div>
                `;
                table.appendChild(row);
            };
            
            container.innerHTML = table.innerHTML || '<div>No processes found</div>';
        })
        .catch(error => console.error('Error:', error));
}

document.addEventListener('DOMContentLoaded', () => {
    setInterval(updateProcesses, 1000);
    updateProcesses(); // Первый запрос
});