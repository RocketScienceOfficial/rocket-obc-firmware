using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UITab : MonoBehaviour
{
    private static readonly Color BASE_COLOR = new(0.25f, 0.25f, 0.25f);
    private static readonly Color ACTIVATED_COLOR = new(0.55f, 0.55f, 0.55f);

    private Image _image;
    private Button _button;

    public Button Button => _button;

    private void Awake()
    {
        _image = GetComponent<Image>();
        _button = GetComponent<Button>();

        SetActivation(false);
    }

    public void SetActivation(bool activation)
    {
        _image.color = activation ? ACTIVATED_COLOR : BASE_COLOR;
        _button.enabled = !activation;
    }
}