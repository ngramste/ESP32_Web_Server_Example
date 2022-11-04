function updateAPI (pin) {
  var xhr = new XMLHttpRequest();
  xhr.withCredentials = true;

  var value = document.getElementById("gpio_" + pin).checked?1:0;

  xhr.addEventListener("readystatechange", function() {
    if(this.readyState === 4) {
      var states = JSON.parse(this.responseText);
      console.log(states);
    }
  });

  xhr.open("POST", "./api?p="+pin+"&v="+value);
  xhr.send();
}

function buildInterface() {
  var pin = 0;
  var interface = document.getElementById("pinInterface");

  interface.innerHTML = "";

  for (pin = 2; pin < 24; pin++) {
    if (pin < 3 || pin >= 12) {
      interface.innerHTML += "<tr><td><label>GPIO " + pin + "</label></td><td><label class=\"switch\"><input type=\"checkbox\" onclick=\"updateAPI(" + pin + ")\" id=\"gpio_" + pin + "\"><span class=\"slider round\"></span></label></td></tr>";
    }
  }
}

buildInterface ();
