using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaserSpawner : MonoBehaviour
{
    [Header("Spawner Settings")]

    [Tooltip("The prefab for the laser emitter game object.")]
    [SerializeField] private GameObject laserPrefab;

    [Tooltip("The time between each laser spawn.")]
    [SerializeField] private float spawnRate = 1f;

    [Tooltip("Amount of lasers travelling across.")]
    [SerializeField] private UInt32 lasersOnLineAtTime = 2;

    float timer = 0;

    [Header("Laser Settings")]

    [Tooltip("The start and end point for the lasers. Don't edit here, set the child transforms in object.")]
    [SerializeField] private Transform[] points;

    [Tooltip("The direction the lasers should be facing.")]
    [SerializeField] private Direction laserFacingDirection = Direction.Down;

    private void Update()
    {
        TimedLaserSpawning();
    }

    /// <summary>
    /// Spawns the lasers in time with the spawnrate.
    /// </summary>
    private void TimedLaserSpawning()
    {
        timer += Time.deltaTime;

        if (timer >= spawnRate)
        {
            SpawnLaser();

            timer = 0;
        }
    }

    /// <summary>
    /// Spawns a laser and applies the laser settings set in the spawner. (Doesn't work if the points array doesn't hold exactly 2 points).
    /// </summary>
    private void SpawnLaser()
    {
        if (points.Length != 4) return;

        GameObject go = Instantiate(laserPrefab, points[0].position, Quaternion.identity);
        MovingPlatform mP = go.GetComponent<MovingPlatform>();

        switch (laserFacingDirection)
        {
            case Direction.Up:
                go.transform.localEulerAngles = new Vector3(0, 0, 180);
                break;
            case Direction.Down:
                go.transform.localEulerAngles = new Vector3(0, 0, 0);
                break;
            case Direction.Left:
                go.transform.localEulerAngles = new Vector3(0, 180, 90);
                break;
            case Direction.Right:
                go.transform.localEulerAngles = new Vector3(0, 180, -90);
                break;
        }

        float dis = Vector2.Distance(points[1].position, points[2].position);

        mP.points = points;
        mP.speed = (dis / spawnRate) / lasersOnLineAtTime;
        mP.destroyOnTarget = true;
    }

}
