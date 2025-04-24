import { invoke } from "@tauri-apps/api/core";

// let greetInputEl: HTMLInputElement | null;
let greetMsgEl: HTMLElement | null;

async function greet() {
  if (greetMsgEl) { // (greetMsgEl && greetInputEl) {
    // Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
    let da: Array<string> = await invoke("my_custom_command2");
    let iter = 1;
    greetMsgEl.innerHTML = ``;
    for (const i in da) {
      if (iter == 1) {
        greetMsgEl.innerHTML += `<div>${da[i]} ${da[Number(i) + 1]}
        ${da[Number(i) + 2]} ${da[Number(i) + 3]}</div>`;
        iter += 1;
      }
      else if (iter == 4) {
        iter = 1;
      }
      else {
        iter += 1;
      }
    }
  }
}

window.addEventListener("DOMContentLoaded", () => {
  // greetInputEl = document.querySelector("#greet-input");
  greetMsgEl = document.querySelector("#processes");
  setInterval(greet, 5000);
  greet();
});