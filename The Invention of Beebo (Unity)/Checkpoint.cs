using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

[RequireComponent(typeof(Collider2D))]
public class Checkpoint : MonoBehaviour
{

    [Tooltip("The place where the player respawns once reset from checkpoint.")]
    public Transform spawnTransform;

    private Collider2D checkpointCollider;
    [SerializeField] private UnityEvent onCheckPoint;

    [SerializeField] private GameObject particleSystemGO;
    [SerializeField] private float celebrateTime = 0.2f;

    [SerializeField] private AudioClip checkpointNoise;

    [SerializeField] private Animator animator;

    private int index = 0;

    private bool triggered;


    private void Start()
    {
        checkpointCollider = GetComponent<Collider2D>();
        checkpointCollider.isTrigger = true;

        if (particleSystemGO) particleSystemGO.SetActive(false);

        if (CheckpointManager.Instance)
        {
            index = CheckpointManager.Instance.GetCheckpointIndex(this);
        }
        else
        {
            Debug.Log("No static CheckpointManager allocated.");
        }
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (other.CompareTag("Player") && CheckpointManager.Instance)
        {
            CheckpointManager.Instance.ChangeCheckpoint(index);

            if (!triggered)
            {
                Debug.Log("tRIGGER");
                onCheckPoint.Invoke();

                if (Singleton_SoundManager.instance && checkpointNoise) Singleton_SoundManager.instance.PlaySingleShot(checkpointNoise);

                if (animator) animator.SetTrigger("Hit");

                StartCoroutine(CelebrationVisual());
                triggered = true;
            }
        }
        else if (other.CompareTag("Player") && !CheckpointManager.Instance)
        {
            Debug.Log("No static CheckpointManager allocated.");
        }
    }

    private IEnumerator CelebrationVisual()
    {
        if (particleSystemGO)
        {
            ParticleSystem particleSystem = particleSystemGO.GetComponent<ParticleSystem>();

            particleSystemGO.SetActive(true);

            if (particleSystem) particleSystem.Play();
        }

        yield return new WaitForSeconds(celebrateTime);

        if (particleSystemGO) particleSystemGO.SetActive(false);

    }
}
