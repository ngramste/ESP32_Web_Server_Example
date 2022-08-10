const gpioCount = 36;

function updateAPI (pin) {
  var xhr = new XMLHttpRequest();
  xhr.withCredentials = true;

  var value = document.getElementById("gpio_" + pin).checked?1:0;
  console.log(pin, value);

  xhr.addEventListener("readystatechange", function() {
    if(this.readyState === 4) {
      console.log(this.responseText);
    }
  });

  xhr.open("POST", "./api?p="+pin+"&v="+value);
  xhr.send();
}

function getStates () {
  var xhr = new XMLHttpRequest();
  xhr.withCredentials = true;

  xhr.addEventListener("readystatechange", function() {
    if(this.readyState === 4) {
      console.log(this.responseText);
    }
  });

  xhr.open("POST", "./api");
  xhr.send();
}

function buildInterface() {
  var pin = 0;
  var interface = document.getElementById("pinInterface");

  interface.innerHTML = "";

  for (pin = 0; pin < gpioCount; pin++) {
    if (pin != 6) {
      interface.innerHTML += "<tr><td><label>GPIO " + pin + "</label></td><td><label class=\"switch\"><input type=\"checkbox\" onclick=\"updateAPI(" + pin + ")\" id=\"gpio_" + pin + "\"><span class=\"slider round\"></span></label></td></tr>";
    }
  }
}

buildInterface ();
