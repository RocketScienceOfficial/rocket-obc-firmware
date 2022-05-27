const express = require("express");
const { SerialPort } = require("serialport");

const port = new SerialPort({
  path: "/dev/ttyACM0",
  baudRate: 9600,
});

port.on('readable', function () {
  const txt = port.read();
  console.log(new Buffer.from(txt).toString());
});
