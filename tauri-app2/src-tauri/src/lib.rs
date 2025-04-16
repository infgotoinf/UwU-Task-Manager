// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
use std::env;

// fn main() -> std::io::Result<()> {
//     let path = env::current_dir()?;
//     println!("The current directory is {}", path.display());
//     Ok(())
// }

// #[tauri::command]
// fn greet(name: &str) -> String {
//     format!("Hello, {}! You've been greeted from Rust!", name)
// }

// #[tauri::command]
// fn my_custom_command() -> String {
//     serde_json::to_string(&env::current_dir()).into()
// }

#[tauri::command]
fn my_custom_command2() -> String {
    let path = env::current_dir().unwrap();
    format!("{}\\..\\..\\dll\\Dll1\\x64\\Debug\\Dll1.dll", path.display()).to_string()
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_opener::init())
        .invoke_handler(tauri::generate_handler![my_custom_command2])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
