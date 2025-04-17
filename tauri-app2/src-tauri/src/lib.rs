// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
use std::{env, ffi::{CStr, c_char}};

// #[tauri::command]
// fn my_custom_command() -> String {
//     serde_json::to_string(&env::current_dir()).into()
// }

struct Process{
    name: String,
    pid: String,
    mem_use: String,
    cpu_use: String
}

impl Process
{
    fn new(name : &str, pid : &str, mem_use : &str, cpu_use : &str) -> Self
    {
        Self {name : name.to_owned(), pid : pid.to_owned(), mem_use : mem_use.to_owned(), cpu_use : cpu_use.to_owned()}
    }
}

#[tauri::command]
fn my_custom_command2() -> String {
    unsafe {
        let path = env::current_dir().unwrap();
        let str_path = format!("{}\\..\\..\\dll\\Dll1\\x64\\Debug\\Dll1.dll", path.display()).to_string();
        let lib = libloading::Library::new(str_path).unwrap();
        let func: libloading::Symbol<unsafe extern "C" fn() -> *const c_char> = lib.get(b"PrintProcessInfo").expect("ErrorLOL");
        let ptr_result = func();
        let c_result = CStr::from_ptr(ptr_result);
        c_result.to_string_lossy().into_owned()
    }
}
// fn my_custom_command2() -> String {
//     let path = env::current_dir().unwrap();
//     format!("{}\\..\\..\\dll\\Dll1\\x64\\Debug\\Dll1.dll", path.display()).to_string()
// }

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_opener::init())
        .invoke_handler(tauri::generate_handler![my_custom_command2])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
