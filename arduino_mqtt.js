const mqtt = require('mqtt');
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

//setting up the Serial Port and linking it to HiveMQ to activate MQTT
const port = new SerialPort('COM4', { baudRate: 9600 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));
const client = mqtt.connect("mqtt://broker.hivemq.com:1883");

client.on('connect', () => {
  client.subscribe('CCTV-ROVER');
  console.log('mqtt connected');
});

const topic = 'CCTV-ROVER';
//const msg = 'Hello MQTT world!';


// Read the port data
port.on("open", () => {
  console.log('serial port open');
});

//Comment out this section if we are using HiveMQ Client to communicate with robot
// Got a message from the Arduino
parser.on('data', data =>{
  console.log('got word from arduino:', data);
  client.publish(topic, data, () => {
    console.log('published the message to MQTT...');
  });
});

//receive message and print it out
client.on('message', (topic, message) => {
  console.log(`Received message on ${topic}: ${message}`);
});


client.on('message', (topic, message) => {
  if (topic == 'CCTV-ROVER') {
      msg = `${message}\n`;
      console.log(msg);

    //now send it to the Arduino
      port.write(msg, (err) => {
        if (err) {
          return console.log('Error on write: ', err.message);
        }
      console.log('message written');
    });
  }
});
