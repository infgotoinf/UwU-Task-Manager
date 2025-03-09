const get_data_url = '/get_data/'; // Корректный URL
const container = document.getElementById('number-container');

function updateNumber() {
    fetch(get_data_url)
        .then(response => response.json())
        .then(data => {
            container.textContent = data.number; // Обновляем число
        })
        .catch(error => console.error('Ошибка:', error));
}

document.addEventListener('DOMContentLoaded', () => {
    updateNumber(); // Первый запрос
    setInterval(updateNumber, 1000); 
});