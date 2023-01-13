using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : DataRecipient
{
    private Vector3 _startPos;
    private Vector3 _basePos;
    private Vector3 _destPos;
    private float _posSpeed;
    private float _positionT;

    private void Start()
    {
        _startPos = transform.position;
        _positionT = 1f;
    }

    private void Update()
    {
        if (_positionT < 1f)
        {
            transform.position = Vector3.Lerp(_basePos, _destPos, _positionT);

            _positionT += Time.deltaTime * _posSpeed;
        }
    }

    public override void OnSetData(RecipentData data)
    {
        var veloctiy = Mathf.Sqrt(data.velocityX * data.velocityX + data.velocityY * data.velocityY + data.velocityZ * data.velocityZ);
        var newPos = _startPos + new Vector3(data.positionX, data.altitude, data.positionZ);

        _basePos = transform.position;
        _destPos = newPos;
        _posSpeed = (newPos.y - _basePos.y) / veloctiy;
        _positionT = 0f;
    }
}