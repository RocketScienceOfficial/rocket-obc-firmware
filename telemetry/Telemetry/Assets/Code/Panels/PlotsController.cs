using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlotsController : DataRecipient
{
    [SerializeField] private PlotPanelController m_AccelerationsPlot;
    [SerializeField] private PlotPanelController m_VelocitiesPlot;
    [SerializeField] private PlotPanelController m_RotationsPlot;
    [SerializeField] private PlotPanelController m_TemperaturePlot;
    [SerializeField] private PlotPanelController m_PressurePlot;

    public override void OnSetData(RecipentData data)
    {
        m_AccelerationsPlot.SetValue(0, data.accelerationX);
        m_AccelerationsPlot.SetValue(1, data.accelerationY);
        m_AccelerationsPlot.SetValue(2, data.accelerationZ);
        m_VelocitiesPlot.SetValue(0, data.velocityX);
        m_VelocitiesPlot.SetValue(1, data.velocityY);
        m_VelocitiesPlot.SetValue(2, data.velocityZ);
        m_RotationsPlot.SetValue(0, data.roll);
        m_RotationsPlot.SetValue(1, data.pitch);
        m_RotationsPlot.SetValue(2, data.yaw);
        m_TemperaturePlot.SetValue(0, data.temperature);
        m_PressurePlot.SetValue(0, data.pressure);
    }
}