using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class DataRecipient : MonoBehaviour
{
    public abstract void OnSetData(RecipentData data);
}

public struct RecipentData
{
    public float accelerationX;
    public float accelerationY;
    public float accelerationZ;
    public float velocityX;
    public float velocityY;
    public float velocityZ;
    public float positionX;
    public float positionY;
    public float positionZ;
    public float roll;
    public float pitch;
    public float yaw;
    public float latitude;
    public float longitude;
    public float altitude;
    public float temperature;
    public float pressure;
    public float humidity;
    public int radioSignalStrength;
    public float cpuTemp;
    public float batteryPercentage;
    public float fuelPercentage;
}