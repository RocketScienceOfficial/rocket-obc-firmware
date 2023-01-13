using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RenderPanelController : DataRecipient
{
    [SerializeField] private RadialPanelController m_VelocityPanel;
    [SerializeField] private RadialPanelController m_AltitudePanel;

    public override void OnSetData(RecipentData data)
    {
        var velocity = Mathf.Sqrt(data.velocityX * data.velocityX + data.velocityY * data.velocityY + data.velocityZ * data.velocityZ);

        m_VelocityPanel.SetValue(velocity, 0f, 1500f);
        m_AltitudePanel.SetValue(data.altitude, 0f, 3000f);
    }
}