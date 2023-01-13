using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class PlotPanelController : MonoBehaviour
{
    private static readonly Color[] VALUES_COLORS = new Color[] { Color.red, Color.green, Color.cyan };

    private const float VERTICAL_POS_OFFSET = 30f;
    private const float VERTICAL_ZERO_OFFSET = -108.5f;
    private const float HORIZONTAL_POS_OFFSET = 55f;

    private HorizontalText[] _horizontalTexts;
    private VerticalText[] _verticalTexts;
    private ValueData[] _values;
    private BottomInfo[] _infos;

    private int _xOffset = 5;
    private int _xShift = 0;

    private int _yOffset = 5;
    private int _minY = 0;
    private int _maxY = 0;
    private int _valueShift = 0;

    private int[] _currentValuesIndexes;

    private bool _initialized;

    private void Init()
    {
        SetupHorizontalTexts();
        SetupVerticalTexts();
        SetupValues();
        SetupBottomInfo();

        UpdateHorizontalTexts();
        UpdateVerticalTexts();
    }

    private void SetupHorizontalTexts()
    {
        var horizontalTextsParent = transform.Find("Panel/Horizontal Texts");

        _horizontalTexts = new HorizontalText[horizontalTextsParent.childCount];

        for (var i = 0; i < horizontalTextsParent.childCount; i++)
        {
            _horizontalTexts[i] = new HorizontalText(horizontalTextsParent.GetChild(i));
        }
    }

    private void SetupVerticalTexts()
    {
        var verticalTextsParent = transform.Find("Panel/Vertical Texts");

        _verticalTexts = new VerticalText[verticalTextsParent.childCount];

        for (var i = 0; i < verticalTextsParent.childCount; i++)
        {
            _verticalTexts[i] = new VerticalText(verticalTextsParent.GetChild(i));
        }
    }

    private void SetupValues()
    {
        var valuesParent = transform.Find("Panel/Values");

        _values = new ValueData[valuesParent.childCount];

        for (var i = 0; i < valuesParent.childCount; i++)
        {
            _values[i] = new ValueData(valuesParent.GetChild(i));
        }

        _currentValuesIndexes = new int[_values[0].Bullets.Count];

        _maxY = _minY + _yOffset * (_verticalTexts.Length - 1);
    }

    private void SetupBottomInfo()
    {
        var infosParent = transform.Find("Info");

        _infos = new BottomInfo[infosParent.childCount];

        for (var i = 0; i < infosParent.childCount; i++)
        {
            _infos[i] = new BottomInfo(infosParent.GetChild(i), i);
        }
    }

    private void UpdateHorizontalTexts()
    {
        for (var i = 0; i < _horizontalTexts.Length; i++)
        {
            _horizontalTexts[i].SetText(_xShift + _xOffset * i);
        }
    }

    private void UpdateVerticalTexts()
    {
        for (var i = 0; i < _verticalTexts.Length; i++)
        {
            _verticalTexts[i].SetValue(_minY + _yOffset * i);
        }
    }

    private void Rescale()
    {
        _yOffset = Mathf.CeilToInt((float)(_maxY - _minY) / (_verticalTexts.Length - 1));

        if (_minY < 0)
        {
            _valueShift = -_minY;
        }

        for (var i = 0; i < _values.Length; i++)
        {
            for (var j = 0; j < _currentValuesIndexes.Length; j++)
            {
                if (_values[i].Actives.TryGetValue(j, out var active) && active)
                {
                    _values[i].SetValue(j, _values[i].Values[j], _valueShift, _yOffset);

                    if (i > 0)
                    {
                        _values[i - 1].SetLine(j, _values[i].Values[j], _valueShift, _yOffset);
                    }
                }
            }
        }
        UpdateVerticalTexts();
    }


    public void SetValue(int index, float value)
    {
        if (!_initialized)
        {
            Init();

            _initialized = true;
        }

        if (_currentValuesIndexes[index] == _values.Length)
        {
            for (var i = 1; i < _values.Length; i++)
            {
                _values[i - 1].SetValue(index, _values[i].Values[index], _valueShift, _yOffset);

                if (i > 1)
                {
                    _values[i - 1 - 1].SetLine(index, _values[i].Values[index], _valueShift, _yOffset);
                }
            }

            _values[_currentValuesIndexes[index] - 1].SetValue(index, value, _valueShift, _yOffset);
            _values[_currentValuesIndexes[index] - 1 - 1].SetLine(index, value, _valueShift, _yOffset);

            _xShift += _xOffset;

            UpdateHorizontalTexts();
        }
        else
        {
            _values[_currentValuesIndexes[index]].SetValue(index, value, _valueShift, _yOffset);

            if (_currentValuesIndexes[index] > 0)
            {
                _values[_currentValuesIndexes[index] - 1].SetLine(index, value, _valueShift, _yOffset);
            }

            _currentValuesIndexes[index]++;
        }

        _infos[index].SetValue(value);

        if (value > _maxY)
        {
            _maxY = (int)value;

            Rescale();
        }
        else if (value < _minY)
        {
            _minY = (int)value;

            Rescale();
        }
    }


    private class HorizontalText
    {
        private readonly TextMeshProUGUI _text;

        public HorizontalText(Transform parent)
        {
            _text = parent.GetComponentInChildren<TextMeshProUGUI>();
        }

        public void SetText(int value)
        {
            _text.SetText(value.ToString());
        }
    }

    private class VerticalText
    {
        private readonly TextMeshProUGUI _text;

        public VerticalText(Transform parent)
        {
            _text = parent.GetComponentInChildren<TextMeshProUGUI>();
        }

        public void SetValue(int value)
        {
            _text.SetText(value.ToString());
        }
    }

    private class ValueData
    {
        public readonly List<Image> Bullets = new();
        public readonly List<Image> Lines = new();

        public readonly Dictionary<int, bool> Actives = new();
        public float[] Values { get; private set; }

        public ValueData(Transform parent)
        {
            var images = parent.GetComponentsInChildren<Image>();
            var bulletIndex = 0;

            for (var i = 0; i < images.Length; i++)
            {
                var img = images[i];

                if (img.gameObject.name.Contains("Bullet"))
                {
                    Bullets.Add(img);

                    img.color = VALUES_COLORS[bulletIndex++];
                }
                else if (img.gameObject.name.Contains("Line"))
                {
                    Lines.Add(img);
                }

                img.gameObject.SetActive(false);
            }

            Values = new float[Bullets.Count];
        }

        public void SetValue(int index, float value, float valueShift, float valueOffset)
        {
            var bullet = Bullets[index];

            bullet.gameObject.SetActive(true);

            Values[index] = value;
            Actives[index] = true;

            var currentPos = bullet.transform.localPosition;
            var yPos = VERTICAL_ZERO_OFFSET + (valueShift + value) / valueOffset * VERTICAL_POS_OFFSET;

            bullet.transform.localPosition = new Vector3(currentPos.x, yPos, currentPos.z);
        }

        public void SetLine(int index, float destValue, float valueShift, float valueOffset)
        {
            var line = Lines[index];

            line.gameObject.SetActive(true);

            var currentPos = line.transform.localPosition;
            var yPos1 = VERTICAL_ZERO_OFFSET + (valueShift + Values[index]) / valueOffset * VERTICAL_POS_OFFSET;
            var yPos2 = VERTICAL_ZERO_OFFSET + (valueShift + destValue) / valueOffset * VERTICAL_POS_OFFSET;

            line.transform.localPosition = new Vector3(currentPos.x, yPos1, currentPos.z);

            var xDist = HORIZONTAL_POS_OFFSET;
            var yDist = yPos2 - yPos1;
            var dist = Mathf.Sqrt(xDist * xDist + yDist * yDist);

            line.transform.eulerAngles = new Vector3(0f, 0f, Mathf.Atan2(yDist, xDist) * Mathf.Rad2Deg);

            var rectTransform = line.GetComponent<RectTransform>();

            rectTransform.sizeDelta = new Vector2(dist, rectTransform.sizeDelta.y);
        }
    }

    private class BottomInfo
    {
        private readonly Image _colorImage;
        private readonly TextMeshProUGUI _valueText;

        public BottomInfo(Transform parent, int i)
        {
            _colorImage = parent.Find("Color Image").GetComponent<Image>();
            _valueText = parent.Find("Value Text").GetComponent<TextMeshProUGUI>();

            _colorImage.color = VALUES_COLORS[i];

            SetValue(0f);
        }

        public void SetValue(float value)
        {
            _valueText.SetText(MathUtils.NumberTwoDecimalPlaces(value));
        }
    }
}