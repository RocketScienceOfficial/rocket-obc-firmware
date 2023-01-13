using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class RadialPanelController : MonoBehaviour
{
    [SerializeField] private TextMeshProUGUI m_ValueText;
    [SerializeField] private Image m_FillImage;

    private float _maxFill;
    private bool _initialized;

    private void Init()
    {
        _maxFill = transform.Find("Panel").GetComponent<Image>().fillAmount;
    }

    public void SetValue(float value, float minValue, float maxValue)
    {
        if (!_initialized)
        {
            Init();

            _initialized = true;
        }

        if (value < minValue || value > maxValue)
        {
            return;
        }

        m_ValueText.SetText(MathUtils.NumberOneDecimalPlace(value));
        m_FillImage.fillAmount = (value - minValue) / (maxValue - minValue) * _maxFill;
    }
}