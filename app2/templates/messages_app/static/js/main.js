document.addEventListener("DOMContentLoaded", () => {
    const resizers = document.querySelectorAll(".header-row .resizer");

    resizers.forEach((resizer, index) => {
        let startX, startWidth;

        resizer.addEventListener("mousedown", (event) => {
            event.preventDefault();
            startX = event.clientX;
            const headerDiv = resizer.parentElement;
            startWidth = headerDiv.offsetWidth;

            function onMouseMove(e) {
                let newWidth = startWidth + (e.clientX - startX);
                if (newWidth > 50 && newWidth < 500) {
                    const allCells = document.querySelectorAll(`.header-row div:nth-child(${index + 1}), .process-row div:nth-child(${index + 1})`);
                    allCells.forEach(cell => {
                        cell.style.flex = `0 0 ${newWidth}px`;
                    });
                }
            }

            function onMouseUp() {
                document.removeEventListener("mousemove", onMouseMove);
                document.removeEventListener("mouseup", onMouseUp);
            }

            document.addEventListener("mousemove", onMouseMove);
            document.addEventListener("mouseup", onMouseUp);
        });
    });
});