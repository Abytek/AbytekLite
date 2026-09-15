console.log("Executed DevelopmentDataServer.js");

const IMPORT_JS_FILE = process.argv[2];
const SIGNATURE = process.argv[3];

console.log("IMPORT_JS_FILE:", IMPORT_JS_FILE);
console.log("SIGNATURE:", SIGNATURE);

const { Server } = require('socket.io');
const express = require('express');
const http = require('http');
const fs = require("fs");
const os = require("os");
const path = require("path");

const app = express();
const server = http.createServer(app);

const io = new Server(server);

class F_DevelopmentDataServer
{
  constructor()
  {
    this.httpServer = server;
    this.socketIOServer = io;

    let Contexts = [];
    let AbytekEBTListFiles = require(IMPORT_JS_FILE);
    for (let AbytekEBTListFile of AbytekEBTListFiles) 
    {
        let List = require(AbytekEBTListFile);
        Contexts.push(require(List.JSContextFile));
    }
    this.AbytekEBTListFiles = AbytekEBTListFiles;

    // Module names
    let ModuleNames = [];
    for (let Context of Contexts)
    {
        for (let ModuleName in Context.ModuleFiles)
        {
            ModuleNames.push(ModuleName);
        }
    }
    this.ModuleNames = ModuleNames;

    // Module files
    let ModuleFiles = new Object();
    for (let Context of Contexts)
    {
        for (let ModuleName in Context.ModuleFiles)
        {
            ModuleFiles[ModuleName] = Context.ModuleFiles[ModuleName];
        }
    }
    this.ModuleFiles = ModuleFiles;

    // Modules
    let Modules = new Object();
    for (let Context of Contexts)
    {
        for (let ModuleName in Context.ModuleFiles)
        {
            Modules[ModuleName] = Context.RequireModule(ModuleName);
        }
    }
    this.Modules = Modules;
  }
}
let DevelopmentDataServer = new F_DevelopmentDataServer();

app.get('/', (req, res) => {
  res.send('<h1>Hello, I am Development Data Server</h1>');
});

app.get('/module_names', (req, res) => {
  res.send(DevelopmentDataServer.ModuleNames);
});
app.get('/module_files', (req, res) => {
  res.send(DevelopmentDataServer.ModuleFiles);
});

io.on('connection', (socket) => {
  console.log('a user connected');
  
  socket.on('module_names', (Ack) => {
    Ack(200, ...DevelopmentDataServer.ModuleNames);
  });

  socket.on('access_module', (ModuleName, AccessCode, Ack) => {
    let Module = DevelopmentDataServer.Modules[ModuleName];
    if (!Module) {
      Ack(400, 'Module not found');
      return;
    }
    if (!AccessCode)
    {
      Ack(400, 'Access code is required');
      return;
    }
    console.log(`Access module ${ModuleName}:`, AccessCode);
    let AccessFunction = Function("Module", AccessCode);
    try
    {
      const Result = AccessFunction(Module);
      Ack(200, Result);
    }
    catch (error)
    {
      Ack(500, error.message);
      return;
    }
  });

  socket.on('execute_js', (JSCode, Ack) => {
    (new Function("DevelopmentDataServer", "Ack", JSCode))(DevelopmentDataServer, Ack);
  });
});

function WritePortFiles(port, signature) {

  const dir = path.join(__dirname, "Temp");
  if (!fs.existsSync(dir))
  {
    fs.mkdirSync(dir, { recursive: true });
  }
  const portFile = path.join(dir, `node_${signature}.port`);
  const doneFile = path.join(dir, `node_${signature}.port_done`);

  // 🔹 write port file atomically
  const portTmp = portFile + ".tmp";
  fs.writeFileSync(portTmp, String(port));
  fs.renameSync(portTmp, portFile); // atomic

  // 🔹 write done file (signal)
  const doneTmp = doneFile + ".tmp";
  fs.writeFileSync(doneTmp, "ok");
  fs.renameSync(doneTmp, doneFile); // atomic
}

server.listen(0, () => {
  const port = server.address().port;
  WritePortFiles(port, SIGNATURE);
  console.log(`listening on port:${port}`);
});