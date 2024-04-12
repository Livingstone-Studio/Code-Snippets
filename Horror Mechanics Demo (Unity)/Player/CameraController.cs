using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{

    [SerializeField] private Transform orientation;

    [SerializeField] private Vector2 sensitivity;

    [SerializeField] private float multiplier = 0.1f;

    private Vector2 mousePos;

    private float xRotation;
    private float yRotation;

    private Vector3 basePos;

    private void Start()
    {
        basePos = transform.localPosition;
    }

    // Update is called once per frame
    void Update()
    {
        // Controls the camera via mouse...
        if (InputHandler.Instance.inputType == InputType.FPS)
        {
            MouseController();
        }
        else if (InputHandler.Instance.inputType == InputType.INSPECTINGKEYCODE)
        {
            FocusOnKeypad(InputHandler.Instance.currentCodeReader);
        }
    }

    private void MouseController()
    {
        if (InputHandler.Instance.inputType != InputType.FPS) return;

        if (transform.localPosition != basePos)
        {
            transform.localPosition = Vector3.MoveTowards(transform.localPosition, basePos, Time.deltaTime * 5);
        }

        mousePos.x = Input.GetAxis("Mouse X");
        mousePos.y = Input.GetAxis("Mouse Y");

        yRotation += mousePos.x * sensitivity.x * multiplier;

        xRotation -= mousePos.y * sensitivity.y * multiplier;
        xRotation = Mathf.Clamp(xRotation, -90f, 90f);

        transform.localRotation = Quaternion.Euler(xRotation, 0, 0f);
        orientation.transform.rotation = Quaternion.Euler(0, yRotation, 0f);
    }

    private void FocusOnKeypad(KeyCodeReader keyCodeReader)
    {
        if (!keyCodeReader) return;

        transform.position = Vector3.MoveTowards(transform.position, keyCodeReader.camPosition.position, Time.deltaTime * 5);
        transform.LookAt(keyCodeReader.transform, Vector3.up);
    }
}
