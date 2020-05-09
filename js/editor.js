var biocoderEditor = null;
var BioCoderEditorState = {
  idle: "IDLE",
  locked: "LOCKED",
};

(function () {
  var canvas_parent = document.getElementById("canvas-parent");
  var canvas_shell = document.getElementById("iframe-shell");
  var canvas_renderer = document.getElementById("canvas-renderer");
  var game_tabs = document.getElementById("game-tabs");
  canvas_shell.width = canvas_parent.offsetWidth;
  canvas_shell.height = canvas_parent.offsetHeight - game_tabs.clientHeight;
  canvas_renderer.width = canvas_parent.offsetWidth;
  canvas_renderer.height = canvas_parent.offsetHeight - game_tabs.clientHeight;

  var pg = document.getElementById("playground");
  var tb = document.getElementById("editor-tabs");
  var pt = document.getElementById("editor-tabs-parent");
  var a = pt.offsetHeight;
  var b = tb.offsetHeight;
  pg.style.height = a - b + "px";
  $("#playground div:first-child").addClass("h-100");
})();

$("#downloadModal").on("show.bs.modal", function (event) {
  var modal = $(this);
  modal
    .find("#model-body-content")
    .text(
      "- " +
        gameMachineEditor.getProgrammingLanguage() +
        " programming language"
    );
});

class BioCoderEditor {
  constructor() {
    this.expectedHTMLPageName = "";
    this.hasClangCompiler = false;
    this.progressId = 0;
    this.ticketId = 0;
    this.state = BioCoderEditorState.idle;
    this.editor = null;
    this.data = {
      main_cpp: {
        name: "main_cpp",
        model: null,
        state: null,
        downloaded: false,
        changed: false,
        link_id: "main-cpp-ui-li",
        a_id: "main-cpp-ui-a",
      },
      biocoder_cpp: {
        name: "biocoder_cpp",
        model: null,
        state: null,
        downloaded: false,
        changed: false,
        link_id: "biocoder-cpp-ui-li",
        a_id: "biocoder-cpp-ui-a",
      },
      biocoder_h: {
        name: "biocoder_h",
        model: null,
        state: null,
        downloaded: false,
        changed: false,
        link_id: "biocoder-h-ui-li",
        a_id: "biocoder-h-ui-a",
      },
      graph_cpp: {
        name: "graph_cpp",
        model: null,
        state: null,
        downloaded: false,
        changed: false,
        link_id: "graph-cpp-ui-li",
        a_id: "graph-cpp-ui-a",
      },
      graph_h: {
        name: "graph_h",
        model: null,
        state: null,
        downloaded: false,
        changed: false,
        link_id: "graph-h-ui-li",
        a_id: "graph-h-ui-a",
      },
    };
    this.li_id = null;
    this.a_id = null;
    this.data.main_cpp.model = monaco.editor.createModel("", "cpp");
    this.data.biocoder_cpp.model = monaco.editor.createModel("", "cpp");
    this.data.biocoder_h.model = monaco.editor.createModel("", "cpp");
    this.data.graph_cpp.model = monaco.editor.createModel("", "cpp");
    this.data.graph_h.model = monaco.editor.createModel("", "cpp");
    this.editorInstance = this.data.main_cpp;
    this.editor = monaco.editor.create(document.getElementById("playground"), {
      model: this.editorInstance.model,
      minimap: {
        enabled: false,
      },
      roundedSelection: false,
      scrollBeyondLastLine: false,
      theme: "vs-light",
    });
    this.binding = this.editor.addCommand(
      monaco.KeyMod.CtrlCmd | monaco.KeyCode.KEY_S,
      function () {
        biocoderEditor.saveFile();
      }
    );
    this.data.main_cpp.model.onDidChangeContent((event) => {
      this.data.main_cpp.changed = true;
      biocoderEditor.saveFile();
    });
    this.data.biocoder_cpp.model.onDidChangeContent((event) => {
      this.data.biocoder_cpp.changed = true;
      biocoderEditor.saveFile();
    });
    this.data.biocoder_h.model.onDidChangeContent((event) => {
      this.data.biocoder_h.changed = true;
      biocoderEditor.saveFile();
    });
    this.data.graph_cpp.model.onDidChangeContent((event) => {
      this.data.graph_cpp.changed = true;
      biocoderEditor.saveFile();
    });
    this.data.graph_h.model.onDidChangeContent((event) => {
      this.data.graph_h.changed = true;
      biocoderEditor.saveFile();
    });
    this.issueTicket = () => {
      var id = this.ticketId;
      this.ticketId += 1;
      return id;
    };
    this.waitLock = async (id, work) => {
      if (
        biocoderEditor.state == BioCoderEditorState.locked ||
        this.progressId < id
      ) {
        setTimeout(this.waitLock, 1000, id, work);
        return;
      }
      biocoderEditor.state = BioCoderEditorState.locked;
      await work();
      this.progressId += 1;
      biocoderEditor.state = BioCoderEditorState.idle;
    };
    this.saveFile = () => {
      var id = this.issueTicket();
      this.waitLock(id, () => {
        if (biocoderEditor.editorInstance === biocoderEditor.data.main_cpp) {
          if (biocoderEditor.data.main_cpp.changed) {
            biocoderEditor.writeFile(
              "main.cpp",
              biocoderEditor.data.main_cpp.model.getValue()
            );
          }
        } else if (
          biocoderEditor.editorInstance === biocoderEditor.data.biocoder_cpp
        ) {
          if (biocoderEditor.data.biocoder_cpp.changed) {
            biocoderEditor.writeFile(
              "biocoder.cpp",
              biocoderEditor.data.biocoder_cpp.model.getValue()
            );
          }
        } else if (
          biocoderEditor.editorInstance === biocoderEditor.data.biocoder_h
        ) {
          if (biocoderEditor.data.biocoder_h.changed) {
            biocoderEditor.writeFile(
              "biocoder.h",
              biocoderEditor.data.biocoder_h.model.getValue()
            );
          }
        } else if (
          biocoderEditor.editorInstance === biocoderEditor.data.graph_cpp
        ) {
          if (biocoderEditor.data.graph_cpp.changed) {
            biocoderEditor.writeFile(
              "graph.cpp",
              biocoderEditor.data.graph_cpp.model.getValue()
            );
          }
        } else if (
          biocoderEditor.editorInstance === biocoderEditor.data.graph_h
        ) {
          if (biocoderEditor.data.graph_h.changed) {
            biocoderEditor.writeFile(
              "graph.h",
              biocoderEditor.data.graph_h.model.getValue()
            );
          }
        }
      });
    };
    this.downloadAsync = async () => {
      return new Promise((resolve, reject) => {
        var id = this.issueTicket();
        this.waitLock(id, () => {
          // TODO: actually download
          // finally resolve
          resolve({});
        });
      });
    };
    this.forwardCommand = async (cmd) => {
      return new Promise(async (resolve, reject) => {
        var id = biocoderEditor.issueTicket();
        biocoderEditor.waitLock(id, async () => {
          biocoderEditor.executeCommand(cmd).then((result) => {
            resolve({});
          });
        });
      });
    };
    this.buildAsync = async () => {
      return new Promise(async (resolve, reject) => {
        if (biocoderEditor.hasClangCompiler == false) {
          await biocoderEditor.forwardCommand("wapm install clang");
        }
        if (biocoderEditor.data.graph_cpp.changed == true) {
          await biocoderEditor.forwardCommand(
            "wapm run clang -cc1 -triple wasm32-unkown-wasi -isysroot /sys -internal-isystem /sys/include -emit-obj ./graph.cpp -o ./graph.o -Wno-everything"
          );
        }
        if (biocoderEditor.data.biocoder_cpp.changed == true) {
          await biocoderEditor.forwardCommand(
            "wapm run clang -cc1 -triple wasm32-unkown-wasi -isysroot /sys -internal-isystem /sys/include -emit-obj ./biocoder.cpp -o ./biocoder.o -Wno-everything"
          );
        }
        if (biocoderEditor.data.main_cpp.changed == true) {
          await biocoderEditor.forwardCommand(
            "wapm run clang -cc1 -triple wasm32-unkown-wasi -isysroot /sys -internal-isystem /sys/include -emit-obj ./main.cpp -o ./main.o -Wno-everything"
          );
        }
        biocoderEditor
          .forwardCommand(
            "wapm run wasm-ld -L/sys/lib/wasm32-wasi /sys/lib/wasm32-wasi/crt1.o ./graph.o ./biocoder.o ./main.o -lc -lc++ -lc++abi -o ./main.wasm"
          )
          .then((result) => {
            biocoderEditor.hasClangCompiler = true;
            biocoderEditor.data.main_cpp.changed = false;
            biocoderEditor.data.biocoder_cpp.changed = false;
            biocoderEditor.data.biocoder_h.changed = false;
            biocoderEditor.data.graph_cpp.changed = false;
            biocoderEditor.data.graph_h.changed = false;
            var content = this.readFile("main.cpp", "utf-8");
            var re = /start_protocol\(\"(.*)\"\)/;
            var groups = re.exec(content);
            if (groups.length == 2) {
              biocoderEditor.expectedHTMLPageName = groups[1] + ".htm";
            }
            resolve({});
          });
      });
    };
    this.runAsync = async () => {
      return biocoderEditor.forwardCommand("./main.wasm");
    };
    this.changeTab = (name) => {
      var currentState = this.editor.saveViewState();
      var currentModel = this.editor.getModel();
      if (currentModel === this.data.main_cpp.model) {
        this.data.main_cpp.state = currentState;
      } else if (currentModel === this.data.biocoder_cpp.model) {
        this.data.biocoder_cpp.state = currentState;
      } else if (currentModel === this.data.biocoder_h.model) {
        this.data.biocoder_h.state = currentState;
      } else if (currentModel === this.data.graph_cpp.model) {
        this.data.graph_cpp.state = currentState;
      } else if (currentModel === this.data.graph_h.model) {
        this.data.graph_h.state = currentState;
      }
      $("#" + this.li_id).removeClass("active");
      $("#" + this.a_id).removeClass("active");
      if (name === this.data.main_cpp.name) {
        this.editorInstance = this.data.main_cpp;
        this.editor.setModel(this.data.main_cpp.model);
        this.editor.restoreViewState(this.data.main_cpp.state);
        this.editor.focus();
        this.li_id = this.data.main_cpp.link_id;
        this.a_id = this.data.main_cpp.a_id;
      } else if (name === this.data.biocoder_cpp.name) {
        this.editorInstance = this.data.biocoder_cpp;
        this.editor.setModel(this.data.biocoder_cpp.model);
        this.editor.restoreViewState(this.data.biocoder_cpp.state);
        this.editor.focus();
        this.li_id = this.data.biocoder_cpp.link_id;
        this.a_id = this.data.biocoder_cpp.a_id;
      } else if (name === this.data.biocoder_h.name) {
        this.editorInstance = this.data.biocoder_h;
        this.editor.setModel(this.data.biocoder_h.model);
        this.editor.restoreViewState(this.data.biocoder_h.state);
        this.editor.focus();
        this.li_id = this.data.biocoder_h.link_id;
        this.a_id = this.data.biocoder_h.a_id;
      } else if (name === this.data.graph_cpp.name) {
        this.editorInstance = this.data.graph_cpp;
        this.editor.setModel(this.data.graph_cpp.model);
        this.editor.restoreViewState(this.data.graph_cpp.state);
        this.editor.focus();
        this.li_id = this.data.graph_cpp.link_id;
        this.a_id = this.data.graph_cpp.a_id;
      } else if (name === this.data.graph_h.name) {
        this.editorInstance = this.data.graph_h;
        this.editor.setModel(this.data.graph_h.model);
        this.editor.restoreViewState(this.data.graph_h.state);
        this.editor.focus();
        this.li_id = this.data.graph_h.link_id;
        this.a_id = this.data.graph_h.a_id;
      }
      $("#" + this.li_id).addClass("active");
      $("#" + this.a_id).addClass("active");
    };
    this.deleteFile = (filepath) => {
      try {
        document
          .getElementById("iframe-shell")
          .contentWindow.that.wasmFs.fs.unlinkSync(filepath);
      } catch (err) {}
    };
    this.readFile = (filepath, encoding) => {
      try {
        return document
          .getElementById("iframe-shell")
          .contentWindow.that.wasmFs.fs.readFileSync(filepath, encoding);
      } catch (err) {
        return null;
      }
    };
    this.fileExists = (filepath) => {
      try {
        return document
          .getElementById("iframe-shell")
          .contentWindow.that.wasmFs.fs.existsSync(filepath);
      } catch (err) {
        return false;
      }
    };
    this.writeFile = (filepath, content) => {
      try {
        document
          .getElementById("iframe-shell")
          .contentWindow.that.wasmFs.fs.writeFileSync(filepath, content);
      } catch (err) {
        return false;
      }
    };
    this.listFiles = (folder, matchingExpr) => {
      let result = [];
      try {
        document
          .getElementById("iframe-shell")
          .contentWindow.that.wasmFs.fs.readdirSync(folder)
          .forEach((file) => {
            if (file.match(new RegExp(matchingExpr, "g"))) {
              result.push(file);
            }
          });
      } catch (err) {}
      return result;
    };
    this.openBrowserTabWithFileContent = (filepath) => {
      var page = window.open(
        "",
        "",
        "width=300,height=300,scrollbars=1,resizable=1"
      );
      var content = biocoderEditor.readFile(filepath, "utf-8");
      page.document.open();
      page.document.write(content);
    };
    this.sleepAsync = async (ms) => {
      return new Promise((resolve) => setTimeout(resolve, ms));
    };
    this.waitForExecutionAsync = async () => {
      while (
        document.getElementById("iframe-shell").contentWindow.that.wapm
          .wasmTerminal.wasmTty._input !== ""
      ) {
        await this.sleepAsync(2000);
      }
    };
    this.executeCommand = async (cmd) => {
      try {
        document
          .getElementById("iframe-shell")
          .contentWindow.that.wasmTerminal.runCommand(cmd);
        await this.waitForExecutionAsync();
      } catch (err) {}
    };
  }
}

function onUITabChange(name) {
  biocoderEditor.changeTab(name);
}

async function onUIBuild() {
  return new Promise(async (resolve, reject) => {
    biocoderEditor.buildAsync().then((result) => {
      resolve({});
    });
  });
}

async function onUIRun() {
  return new Promise(async (resolve, reject) => {
    biocoderEditor.buildAsync().then((result) => {
      biocoderEditor.runAsync().then((result) => {
        biocoderEditor.openBrowserTabWithFileContent(
          biocoderEditor.expectedHTMLPageName
        );
        resolve({});
      });
    });
  });
}

window.onload = function () {
  require.config({ paths: { vs: "js/min/vs" } });
  require(["vs/editor/editor.main"], function () {
    biocoderEditor = new BioCoderEditor();
    // main
    fetch("/sources/main.cpp")
      .then((r) => r.text())
      .then((data) => {
        biocoderEditor.data.main_cpp.model.setValue(data);
        biocoderEditor.data.main_cpp.downloaded = true;
        biocoderEditor.data.main_cpp.changed = true;
        biocoderEditor.writeFile("main.cpp", data);
        biocoderEditor.changeTab("main_cpp");
        // graph
        fetch("/sources/graph.cpp")
          .then((r) => r.text())
          .then((data) => {
            biocoderEditor.data.graph_cpp.model.setValue(data);
            biocoderEditor.data.graph_cpp.downloaded = true;
            biocoderEditor.data.graph_cpp.changed = true;
            biocoderEditor.writeFile("graph.cpp", data);
            fetch("/sources/graph.h")
              .then((r) => r.text())
              .then((data) => {
                biocoderEditor.data.graph_h.model.setValue(data);
                biocoderEditor.data.graph_h.downloaded = true;
                biocoderEditor.data.graph_h.changed = true;
                biocoderEditor.writeFile("graph.h", data);
                // biocoder
                fetch("/sources/biocoder.h")
                  .then((r) => r.text())
                  .then((data) => {
                    biocoderEditor.data.biocoder_h.model.setValue(data);
                    biocoderEditor.data.biocoder_h.downloaded = true;
                    biocoderEditor.data.biocoder_h.changed = true;
                    biocoderEditor.writeFile("biocoder.h", data);
                    fetch("/sources/biocoder.cpp")
                      .then((r) => r.text())
                      .then((data) => {
                        biocoderEditor.data.biocoder_cpp.model.setValue(data);
                        biocoderEditor.data.biocoder_cpp.downloaded = true;
                        biocoderEditor.data.biocoder_cpp.changed = true;
                        biocoderEditor.writeFile("biocoder.cpp", data);
                      });
                  });
              });
          });
      });
  });
};
