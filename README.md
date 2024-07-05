# smart-plant-monitor-system

The goal of this project is to develop a smart system that monitors the health of a plant by measuring soil moisture, temperature, humidity, and light levels. Each node will feed the data to backend server and dashboard will showcase which plants need attention. Backend will be using python framework which is to be decided.

## Stages
- [x] Node publish sample json to mqtt broker
- [x] Node integrate with temperature and humid sensor
- [ ] Check into node time update with pc clock via serial
- [ ] Stabilize mqtt connection
- [ ] Create ble node network
- [ ] Node and stm32 connect with each other
- [ ] Stm32 being able to publish data mqtt broker