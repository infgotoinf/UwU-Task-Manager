// templates/src/js/ajax.js 
let get_data_url = 'get_data/' // Переход на ссылку с ajax запросом

const messagesContainer = document.querySelector('#messages-container'); // Сюда помещаются наши статьи

function draw_messages(messages) {
  // Очистим контейнер перед добавлением новых сообщений
  messagesContainer.innerHTML = '';

  // Для каждого сообщения в массиве messages создаем HTML
  messages.forEach(message => {
      const messageElement = document.createElement('div');
      
      // Создаем элементы для отображения данных сообщения
      const senderName = document.createElement('p');
      senderName.textContent = `Отправитель: ${message.sender_name}`;
      
      const messageText = document.createElement('p');
      messageText.textContent = `Сообщение: ${message.message}`;
      
      const sentAt = document.createElement('p');
      sentAt.textContent = `Отправлено: ${message.sent_at}`;
      
      // Создаем разделитель
      const hr = document.createElement('hr');
      
      // Добавляем все элементы в messageElement
      messageElement.appendChild(senderName);
      messageElement.appendChild(messageText);
      messageElement.appendChild(sentAt);
      messageElement.appendChild(hr);
      
      // Вставляем messageElement в контейнер
      messagesContainer.appendChild(messageElement);
  });
}

function ajax_get() {
  fetch(get_data_url, {
      method: 'GET', // Используем метод 'GET'
  })
  .then(response => response.json()) // Преобразуем ответ в JSON
  .then(data => {
      draw_messages(data.messages); // Передаем полученные сообщения в draw_messages
  })
  .catch(error => console.error('Ошибка:', error)); // Обрабатываем ошибки

  setInterval(ajax_get, 10000);
}