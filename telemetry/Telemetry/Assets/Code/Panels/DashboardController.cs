using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class DashboardController : DataRecipient
{
    [SerializeField] private RadialPanelController m_CPUPanel;
    [SerializeField] private RadialPanelController m_BatteryPanel;
    [SerializeField] private RadialPanelController m_SignalPanel;
    [SerializeField] private RadialPanelController m_TemperaturePanel;
    [SerializeField] private RadialPanelController m_PressurePanel;
    [SerializeField] private RadialPanelController m_HumidityPanel;
    [SerializeField] private RadialPanelController m_FuelPanel;
    [SerializeField] private TextMeshProUGUI m_RollText;
    [SerializeField] private TextMeshProUGUI m_PitchText;
    [SerializeField] private TextMeshProUGUI m_YawText;
    [SerializeField] private TextMeshProUGUI m_VelocityText;
    [SerializeField] private TextMeshProUGUI m_AltitudeText;

    public override void OnSetData(RecipentData data)
    {
        var veloctiy = Mathf.Sqrt(data.velocityX * data.velocityX + data.velocityY * data.velocityY + data.velocityZ * data.velocityZ);

        m_CPUPanel.SetValue(data.cpuTemp, 15f, 30f);
        m_BatteryPanel.SetValue(data.batteryPercentage, 0f, 100f);
        m_SignalPanel.SetValue(data.radioSignalStrength, -120f, -30f);
        m_TemperaturePanel.SetValue(data.temperature, -5f, 50f);
        m_PressurePanel.SetValue(data.pressure, 300f, 1100f);
        m_HumidityPanel.SetValue(data.humidity, 0f, 100f);
        m_FuelPanel.SetValue(data.fuelPercentage, 0f, 100f);
        m_RollText.SetText($"Roll: {data.roll}°");
        m_PitchText.SetText($"Pitch: {data.pitch}°");
        m_YawText.SetText($"Yaw: {data.yaw}°");
        m_VelocityText.SetText($"Speed: {veloctiy} m/s");
        m_AltitudeText.SetText($"Altitude: {data.altitude} m");
    }
}