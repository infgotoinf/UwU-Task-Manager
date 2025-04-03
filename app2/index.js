const { app, BrowserWindow } = require('electron')
// include the Node.js 'path' module at the top of your file
// const path = require('node:path')

// modify your existing createWindow() function
const createWindow = () => {
  const win = new BrowserWindow({
    webPreferences: {
        nodeIntegration: true,
        contextIsolation: false,
    },
    width: 800*2,
    height: 600*2,
  })

  win.loadFile('index.html')
}

  app.whenReady().then(() => {
    createWindow()
  })

function ProcessPrinter() {
    var dll = loadLibrary("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll");
    var MessageBox = dll.PrintProcessInfo();
    MessageBox();
}

  app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') app.quit()
  })