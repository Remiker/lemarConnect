function decodeUplink(input) {
    var bytes = input.bytes;
    var data = {};

    // Decode temperature
    var temperature = (bytes[0] << 8) | bytes[1];  // Combine high and low bytes
    data.temperature = temperature / 100;          // Convert to original value (since it was multiplied by 100)

    // Decode humidity
    var humidity = (bytes[2] << 8) | bytes[3];
    data.humidity = humidity / 100;                // Convert to original value

    // Decode pressure
    var pressure = (bytes[4] << 8) | bytes[5];
    data.pressure = pressure / 100;                // Convert to original value

    // Decode illuminance
    var illuminance = (bytes[6] << 8) | bytes[7];
    data.illuminance = illuminance / 100;          // Convert to original value

    return {
        data: data,
        warnings: [],
        errors: []
    };
}
