const get_data_url = '/get_data/'; // Корректный URL
const container = document.getElementById('process-container');

function updateProcesses() {
    fetch(get_data_url)
        .then(response => response.json())
        .then(data => {
            container.textContent = data.processes; // Обновляем число
        })
        .catch(error => console.error('Ошибка:', error));
}

document.addEventListener('DOMContentLoaded', () => {
    updateProcesses(); // Первый запрос
    setInterval(updateProcesses, 1000); 
});