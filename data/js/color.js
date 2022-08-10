
let sel;
let r;
let g;
let b;
let l;
let mutexLock = false;

function updateAPI (el, forced) {
  sel = document.getElementById("selectedColor");
  r = document.getElementById("r").value;
  g = document.getElementById("g").value;
  b = document.getElementById("b").value;
  l = document.getElementById("l").value;

  sel.style.backgroundColor = "rgb("+(r*l/255)+","+(g*l/255)+","+(b*l/255)+")";
  if (!mutexLock || forced) {
    mutexLock = true;
    var xhr = new XMLHttpRequest();
    xhr.withCredentials = true;

    xhr.addEventListener("readystatechange", function() {
      if(this.readyState === 4) {
        mutexLock = false;
        console.log(this.responseText);
        let colors = this.responseText.split(",");
        r = colors[0];
        b = colors[1];
        g = colors[2];
        l = colors[3];
      }
    });

    if (el.id == "l") xhr.open("POST", "./api?l="+l);
    else xhr.open("POST", "./api?r="+r+"&g="+g+"&b="+b+"&l="+l);
    xhr.send();
  }
}

function getCurrentColor () {
  var xhr = new XMLHttpRequest();
  xhr.withCredentials = true;

  xhr.addEventListener("readystatechange", function() {
    if(this.readyState === 4) {
      console.log(this.responseText);
      let colors = this.responseText.split(",");
      r = colors[0];
      b = colors[1];
      g = colors[2];
      l = colors[3];

      document.getElementById("r").value = r;
      document.getElementById("g").value = g;
      document.getElementById("b").value = b;
      document.getElementById("l").value = l;
      document.getElementById("selectedColor").style.backgroundColor = "rgb("+(r*l/255)+","+(g*l/255)+","+(b*l/255)+")";
    }
  });

  xhr.open("POST", "./api");
  xhr.send();
}

function turnoff() {
  document.getElementById("r").value = 0;
  document.getElementById("g").value = 0;
  document.getElementById("b").value = 0;
  document.getElementById("l").value = 0;

  updateAPI(true);
}

function rainbow() {
  var xhr = new XMLHttpRequest();
  xhr.withCredentials = true;

  xhr.addEventListener("readystatechange", function() {
    if(this.readyState === 4) {
      mutexLock = false;
      console.log(this.responseText);
      let colors = this.responseText.split(",");
      r = colors[0];
      b = colors[1];
      g = colors[2];
      l = colors[3];
    }
  });

  xhr.open("POST", "./api?rainbow");
  xhr.send();
}

getCurrentColor();
