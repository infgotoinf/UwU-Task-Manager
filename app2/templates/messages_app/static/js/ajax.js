const get_data_url = '/get_data/';
const container = document.getElementById('process-container');

function updateProcesses() {
    fetch(get_data_url)
        .then(response => response.json())
        .then(data => {
            let html = '';
            const processes = data.processes;
            
            // Перебираем все PID в объекте процессов
            for (const pid in processes) {
                const process = processes[pid];
                html += `
                    <div class="process">
                        <span class="name">Name: ${process.name}</span>
                        <span class="pid">PID: ${pid}</span>
                    </div>
                `;
            }
            
            container.innerHTML = html || '<div>No processes found</div>';
        })
        .catch(error => console.error('Error:', error));
}

document.addEventListener('DOMContentLoaded', () => {
    setInterval(updateProcesses, 1000);
    updateProcesses(); // Первый запрос
});