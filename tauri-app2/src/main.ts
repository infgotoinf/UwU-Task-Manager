import { invoke } from "@tauri-apps/api/core";

// let greetInputEl: HTMLInputElement | null;
let greetMsgEl: HTMLElement | null;

async function greet() {
  if (greetMsgEl) { // (greetMsgEl && greetInputEl) {
    // Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
    let da: Array<string> = await invoke("my_custom_command2");
    greetMsgEl.textContent = da[0];
    // for i in da
  }
}

window.addEventListener("DOMContentLoaded", () => {
  // greetInputEl = document.querySelector("#greet-input");
  greetMsgEl = document.querySelector("#greet-msg");
  document.querySelector("#greet-form")?.addEventListener("submit", (e) => {
    e.preventDefault();
    greet();
  });
});
