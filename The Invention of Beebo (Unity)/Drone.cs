using System.Collections;
using UnityEngine;

public class Drone : MonoBehaviour
{

    [SerializeField] private float minSpeed;
    [SerializeField] private float maxSpeed;

    [SerializeField] private float speedMultiplier = 1.4f;

    private float speed;

    [SerializeField] private Vector3 offset;
    [SerializeField] private Transform target;

    [SerializeField] private bool activated = true;

    [SerializeField] private Collider2D droneCollider;
    [SerializeField] private Rigidbody2D droneRB;

    [SerializeField] private bool startAtPlayer = true;

    private void Start()
    {
        droneCollider = GetComponent<Collider2D>();
        droneRB = GetComponent<Rigidbody2D>();

        droneCollider.enabled = false;
        droneRB.isKinematic = true;

        target = PlayerController.Instance.transform;

        if (startAtPlayer && target)
        {
            ResetToTarget();
        }
    }

    void Update()
    {
        if (activated && target && Time.timeScale != 0)
        {
            // clamp speed depend on distance? closer drone is to target - slower it moves...

            Vector3 adjustedOffset = AdjustedOffset();

            float distance = Vector3.Distance(transform.position, target.position + adjustedOffset);

            speed = Mathf.Clamp(distance * speedMultiplier, minSpeed, maxSpeed);

            transform.position = Vector3.MoveTowards(transform.position, target.position + adjustedOffset, speed * Time.deltaTime);
        }
    }

    public void ResetToTarget()
    {
        if (!target) return;

        transform.position = target.position + AdjustedOffset();
    }

    private Vector3 AdjustedOffset()
    {
        Vector3 adjustedOffset = offset;

        if (target.localScale.x < 0)
        {
            adjustedOffset.x *= -1f;
        }

        return adjustedOffset;
    }

    private void OnDrawGizmos()
    {
        if (!target) return;

        Gizmos.color = Color.black;
        Gizmos.DrawWireSphere(target.position + AdjustedOffset(),0.5f);    
    }

    public void DeactivateForTime(float time)
    {
        if (!activated) return;

        StartCoroutine(DisabledForPeriod(time));
    }

    private IEnumerator DisabledForPeriod(float time)
    {
        Debug.Log("Disabling Drone...");

        activated = false;

        droneCollider.enabled = true;
        droneRB.isKinematic = false;

        DroneTurret.Instance.activated = false;

        yield return new WaitForSeconds(time);

        Debug.Log("Reenabling Drone...");

        activated = true;

        droneCollider.enabled = false;
        droneRB.isKinematic = true;

        DroneTurret.Instance.activated = true;
    }
}
