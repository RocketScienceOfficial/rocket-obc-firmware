using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class DataProvider : MonoBehaviour
{
    [SerializeField] private DataRecipient[] m_Recipents;
    [SerializeField] private SerialPortController m_SerialPortController;

    private void Start()
    {
        m_SerialPortController.OnReadData += (sender, args) =>
        {
            var msg = args.Data;

            if (string.IsNullOrEmpty(msg) || !msg.StartsWith("/*") || !msg.EndsWith("*/"))
            {
                return;
            }

            msg = msg.Remove(0, 2);
            msg = msg.Remove(msg.Length - 2, 2);

            var data = msg.Split(',', StringSplitOptions.RemoveEmptyEntries);

            for (var i = 0; i < data.Length; i++)
            {
                data[i] = data[i].Replace('.', ',');
            }

            var recipentData = new RecipentData
            {
                accelerationX = float.Parse(data[0]),
                accelerationY = float.Parse(data[1]),
                accelerationZ = float.Parse(data[2]),
                velocityX = float.Parse(data[3]),
                velocityY = float.Parse(data[4]),
                velocityZ = float.Parse(data[5]),
                positionX = float.Parse(data[6]),
                positionY = float.Parse(data[7]),
                positionZ = float.Parse(data[8]),
                roll = float.Parse(data[9]),
                pitch = float.Parse(data[10]),
                yaw = float.Parse(data[11]),
                latitude = float.Parse(data[12]),
                longitude = float.Parse(data[13]),
                altitude = float.Parse(data[14]),
                temperature = float.Parse(data[15]),
                pressure = float.Parse(data[16]),
                humidity = float.Parse(data[17]),
                radioSignalStrength = int.Parse(data[18]),
                cpuTemp = float.Parse(data[19]),
                batteryPercentage = float.Parse(data[20]),
                fuelPercentage = float.Parse(data[21]),
            };

            SetData(recipentData);
        };
    }

    private void SetData(RecipentData data)
    {
        foreach (var item in m_Recipents)
        {
            item.OnSetData(data);
        }
    }
}