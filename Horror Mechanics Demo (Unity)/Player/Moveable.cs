using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(CharacterController))]
public class Moveable : MonoBehaviour
{
    [Header("Components")]
    private CharacterController characterController;

    [SerializeField] private Transform orientation;

    [Header("Movement Variables")]

    [SerializeField] private float moveSpeed = 5f;
    [SerializeField] private float moveMultiplier = 0.01f;

    [Header("Grounded Variables")]

    [SerializeField] private float disToGround = 0.7f;
    [SerializeField] private LayerMask groundLayer;

    private bool isGrounded = false;

    // Start is called before the first frame update
    void Start()
    {
        characterController = GetComponent<CharacterController>();
    }

    // Update is called once per frame
    void Update()
    {
        isGrounded = Physics.Raycast(transform.position, Vector3.down, disToGround, groundLayer);

        Gravity();
    }

    public void Movement(Vector2 moveVector)
    {
        ForwardMovement(moveVector.normalized.y);
        SideMovement(moveVector.normalized.x);
    }

    private void ForwardMovement(float dir)
    {
        characterController.Move(orientation.transform.forward * dir * moveMultiplier * moveSpeed * Time.deltaTime);
    }

    private void SideMovement(float dir)
    {
        characterController.Move(orientation.transform.right * dir * moveMultiplier * moveSpeed * Time.deltaTime);
    }

    private void Gravity()
    {
        if (!isGrounded)
        {
            characterController.Move(Vector3.down * moveMultiplier * moveSpeed * Time.deltaTime);
        }
    }
}

// Rigidbody or character controller...

// character controller = easier, rigidbody = more versitile

// no real need for the movement versitility of the rigidbody movement...

// try character controller first?