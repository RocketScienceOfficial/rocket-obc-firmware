using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class MapController : DataRecipient
{
    [SerializeField] private TextMeshProUGUI m_LatitudeText;
    [SerializeField] private TextMeshProUGUI m_LongtitudeText;
    [SerializeField] private TextMeshProUGUI m_AltitudeText;
    [SerializeField] private TextMeshProUGUI m_DisplacementText;

    public override void OnSetData(RecipentData data)
    {
        var displacement = Mathf.Sqrt(data.positionX * data.positionX + data.positionZ * data.positionZ);

        m_LatitudeText.SetText($"{MathUtils.NumberFiveDecimalPlaces(data.latitude)}°");
        m_LongtitudeText.SetText($"{MathUtils.NumberFiveDecimalPlaces(data.longitude)}°");
        m_AltitudeText.SetText($"{MathUtils.NumberOneDecimalPlace(data.altitude)} m");
        m_DisplacementText.SetText($"{MathUtils.NumberOneDecimalPlace(displacement)} m");
    }
}