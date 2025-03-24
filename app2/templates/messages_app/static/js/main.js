document.addEventListener("DOMContentLoaded", () => {
    const table = document.querySelector(".table");
    const killButton = document.querySelector(".btn");
    const resizers = document.querySelectorAll(".resizer");

    // let processes = [
    //     { name: "chrome.exe", pid: 1234, memory: "200MB", cpu: "5%" },
    //     { name: "explorer.exe", pid: 5678, memory: "50MB", cpu: "2%" },
    //     { name: "notepad.exe", pid: 9101, memory: "10MB", cpu: "0%" },
    //     { name: "discord.exe", pid: 1121, memory: "150MB", cpu: "3%" }
    // ];

    // function renderProcesses(processes) {
    //     table.innerHTML = ""; 
    //     for (const pid in processes) {
    //         const row = document.createElement("div");
    //         row.classList.add("process-row");
    //         row.dataset.index = index;
    //         row.innerHTML = `
    //             <div class="process">
    //                 <span class="name">Name: ${process.name}</span>
    //                 <span class="pid">PID: ${pid}</span>
    //                 <span class="memUse">MemUse: ${process.memUse}</span>
    //             </div>
    //         `;
    //         table.appendChild(row);
    //     };
    // }

    // function killLastProcess() {
    //     if (processes.length > 0) {
    //         processes.pop();
    //         renderProcesses();
    //     } else {
    //         alert("Нет процессов для завершения!");
    //     }
    // }

    // killButton.addEventListener("click", killLastProcess);

    // renderProcesses();

    // Функция изменения ширины колонок
    resizers.forEach(resizer => {
        let startX, startWidth, column;

        resizer.addEventListener("mousedown", (event) => {
            startX = event.clientX;
            column = resizer.parentElement;
            startWidth = column.offsetWidth;

            document.addEventListener("mousemove", onMouseMove);
            document.addEventListener("mouseup", onMouseUp);
        });

        function onMouseMove(event) {
            let newWidth = startWidth + (event.clientX - startX);
            if (newWidth > 50 && newWidth < 300) { // Ограничения ширины
                column.style.flex = `0 0 ${newWidth}px`;
            }
        }

        function onMouseUp() {
            document.removeEventListener("mousemove", onMouseMove);
            document.removeEventListener("mouseup", onMouseUp);
        }
    });
});