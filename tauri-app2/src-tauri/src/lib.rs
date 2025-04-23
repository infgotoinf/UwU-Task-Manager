// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
use std::{env, ffi::{CStr, c_char}};

// #[tauri::command]
// fn my_custom_command() -> String {
//     serde_json::to_string(&env::current_dir()).into()
// }

struct Process 
{
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



use tauri::ipc::Response;
#[tauri::command]
fn my_custom_command2() -> Vec<Process>
{
    unsafe
    {
        let path = env::current_dir().unwrap();
        let str_path = format!("{}\\..\\..\\dll\\Dll1\\x64\\Debug\\Dll1.dll", path.display()).to_string();
        let lib = libloading::Library::new(str_path).unwrap();
        let func: libloading::Symbol<unsafe extern "C" fn() -> *const c_char> = lib.get(b"PrintProcessInfo").expect("ErrorLOL");
        let ptr_result = func();
        let c_result = CStr::from_ptr(ptr_result);
        let result = c_result.to_string_lossy(); //.into_owned()

        let sub_result = &result[1..result.len()];
        let mut vec_result      = Vec::new();
        let mut switch          = 0;
        let mut name: String    = "".to_string();
        let mut pid: String     = "".to_string();
        let mut mem_use: String = "".to_string();
        let mut cpu_use: String = "".to_string();
        for i in sub_result.chars()
        {
            //parsed += &i.to_string();
            match switch
            {
            0 =>if i != ':' {
                    mem_use += &i.to_string();
                }
                else {
                    switch = 1;
                }
            1 =>if i != ':' {
                    cpu_use += &i.to_string();
                }
                else {
                    switch = 2;
                }
            2 =>if i != ':' {
                    name += &i.to_string();
                }
                else {
                    switch = 3;
                }
            _ =>if i != ';' {
                    pid += &i.to_string();
                }
                else {
                    switch  = 0;
                    vec_result.push(Process::new(&name, &pid, &mem_use, &cpu_use));
                    name    = "".to_string();
                    pid     = "".to_string();
                    mem_use = "".to_string();
                    cpu_use = "".to_string();
                }
            }
        }
        vec_result
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
