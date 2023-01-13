using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RocketController : DataRecipient
{
    private Vector3 _startPos;
    private Vector3 _basePos;
    private Vector3 _destPos;
    private float _posSpeed;
    private float _positionT;
    private Vector3 _baseAngles;
    private Vector3 _destAngles;
    private float _rotSpeed;
    private float _rotationT;

    private void Start()
    {
        _startPos = transform.position;
        _positionT = 1f;
        _rotationT = 1f;
    }

    private void Update()
    {
        if (_positionT < 1f)
        {
            transform.position = Vector3.Lerp(_basePos, _destPos, _positionT);

            _positionT += Time.deltaTime * _posSpeed;
        }

        if (_rotationT < 1f)
        {
            transform.eulerAngles = Vector3.Lerp(_baseAngles, _destAngles, _rotationT);

            _rotationT += Time.deltaTime * _rotSpeed;
        }
    }

    public override void OnSetData(RecipentData data)
    {
        var veloctiy = Mathf.Sqrt(data.velocityX * data.velocityX + data.velocityY * data.velocityY + data.velocityZ * data.velocityZ);
        var newPos = _startPos + new Vector3(data.positionX, data.altitude, data.positionZ);

        _basePos = transform.position;
        _destPos = newPos;
        _posSpeed = veloctiy != 0f ? (newPos.y - _basePos.y) / veloctiy : 0f;

        _baseAngles = transform.eulerAngles;
        _destAngles = new Vector3(data.pitch, data.roll, data.yaw);
        _rotSpeed = 10f;

        _positionT = 0f;
        _rotationT = 0f;
    }
}