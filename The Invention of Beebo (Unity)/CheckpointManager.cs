using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckpointManager : MonoBehaviour
{
    /// <summary>
     /// Stores both the position and euler angles without having to refer to a transform.
    /// </summary>
    struct PositionAndRotation
    {
        public Vector2 position;
        public Vector3 localEulerAngles;
    }

    public static CheckpointManager Instance { set; get; }

    [Header("Checkpoint Management")]
    
    [Tooltip("The list that should contain the chronologically ordered level checkpoints.")]
    [SerializeField] private List<Checkpoint> checkpointList = new List<Checkpoint>();

    [Tooltip("If this is set to true it will assume the checkpoint list is laid out in order of checkpoints the player visits in the level and that they should not be able to return once past.")]
    [SerializeField] bool linearProgression = true;
    
    private int currentCheckpointIndex = 0;

    [Header("Enemy Management")]

    [Tooltip("The list that should contain all the enemies/respawning damagables in the level.")]
    [SerializeField] private List<GameObject> enemies = new List<GameObject>();

    [Tooltip("The list that should contain all collectables in the scene so a proper reset can be activated.")]
    [SerializeField] private List<GameObject> collectablesCollection = new List<GameObject>();

    private List<PositionAndRotation> enemiesSpawnPos = new List<PositionAndRotation>();

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
        }
        else
        {
            Destroy(gameObject);
        }
    }

    private void Start()
    {
        enemiesSpawnPos.Clear();

        PositionAndRotation enemyPR;
        foreach (GameObject enemy in enemies)
        {
            enemyPR.position = enemy.transform.position;
            enemyPR.localEulerAngles = enemy.transform.localEulerAngles;

            enemiesSpawnPos.Add(enemyPR);
        }
    }

    /// <summary>
    /// Resets the player and enemies once the player dies.
    /// </summary>
    public void ResetFromCheckpoint()
    {
        Checkpoint checkpoint = checkpointList[currentCheckpointIndex];

        // Respawn Player - Should have a reset stats function.

        HealthSystem hs = PlayerController.Instance.GetComponent<HealthSystem>();

        if (hs)
        {
            hs.RestoreHeal(1000);

            UI_Manager.Instance.UpdatePlayerHealthUI(hs.GetNormalisedHealth());
        }
        
        PlayerController.Instance.transform.position = checkpoint.spawnTransform.position;
        DroneTurret.Instance.GetComponentInParent<Drone>().ResetToTarget();
        ClearCollectables();

        PlayerController.Instance.rigidBody2D.velocity = Vector2.zero;

        // Respawn Enemies

        for (int i = 0; i < enemies.Count; i++)
        {
            enemies[i].SetActive(true);

            // (enemies[i].)

            enemies[i].transform.position = enemiesSpawnPos[i].position;
            enemies[i].transform.localEulerAngles = enemiesSpawnPos[i].localEulerAngles;
        }

        if (EnemyManager.Instance) EnemyManager.Instance.ResetEnemies();

    }

    /// <summary>
    /// Changes the current checkpoint to refer to the one just hit.
    /// </summary>
    public void ChangeCheckpoint(int checkpointIndex)
    {
        if (checkpointIndex <= currentCheckpointIndex && linearProgression) return;

        currentCheckpointIndex = checkpointIndex;
    }

    /// <summary>
    /// Finds the place where the player should spawn on the checkpoint.
    /// </summary>
    public Transform GetCurrentSpawnPoint()
    {
        return checkpointList[currentCheckpointIndex].spawnTransform;
    }

    /// <summary>
    /// Finds the checkpoints index in the list.
    /// </summary>
    public int GetCheckpointIndex(Checkpoint checkpoint)
    {
        return checkpointList.IndexOf(checkpoint);
    }

    public void AppendToCollectables(GameObject go)
    {
        collectablesCollection.Add(go);
    }

    public void RemoveFromCollectables(GameObject go)
    {
        if (!collectablesCollection.Contains(go)) return;

        collectablesCollection.Remove(go);
    }

    public void ClearCollectables()
    {
        for (int i = 0; i < collectablesCollection.Count; i++)
        {
            if (collectablesCollection[i] != null)
            {
                Destroy(collectablesCollection[i]);
            }
        }

        collectablesCollection.Clear();
    }
    public Checkpoint GetCheckpoint(int index)
    {
        return checkpointList[index];
    }
        
}
