using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DroneTurret : MonoBehaviour
{
    public static DroneTurret Instance { get; private set; }

    #region Attributes

    [Header("Components")]

    [SerializeField] private Animator animator;

    [Tooltip("The List of available weapon of the drone.")]

    [SerializeField] private List<DroneWeaponSO> avaliableDroneWeaponList;

    [Tooltip("The prefab of the turret's gun barrel.")]

    [SerializeField] private GameObject barrelPrefab;

    [Tooltip("The place on the turret where the bullets get shot from.")]

    [SerializeField] private List<Transform> shootPoints = new List<Transform>(); // Set in the turret prefab;

    [SerializeField] private Transform droneTransform;

    [Header("Turret Settings")]

    [Tooltip("Determines if the turret is turned on or turned off.")]

    public bool activated = true;

    [Tooltip("Locked is used in the lock-on turret type to determine when it should stop tracking the mouse and shoot.")]

    private bool locked = false;

    [Tooltip("The timer that the turret uses to count the cooldowns in between shots.")]

    private float shotTimer;

    [Tooltip("Always tracks current mouse position.")]

    private Vector3 mousePosition;

    [Tooltip("Always tracks the current rotation towards the mouse position from the turret.")]

    private Vector2 aimPos;

    [Tooltip("The current weapon of the drone")]

    private DroneWeaponSO currentDroneWeapon;

    private bool shooting = false;

    [Tooltip("The time it takes to swap between drone weapons.")]

    [SerializeField] private float maxSwitchTimer = 0.2f;

    private float switchTimer;

    #endregion

    private void Awake()
    {
        Instance = this;
    }

    private void Start()
    {
        ActivateWeapon(avaliableDroneWeaponList[0]);

        currentDroneWeapon.ResetStats();
        
        shotTimer = currentDroneWeapon.fireRate;
        aimPos = mousePosition;         
    }

    void Update()
    {
        if(Time.timeScale > 0)
        {
            if (!activated)
            {
                locked = false; 

                if (animator)
                {
                    if (!animator.GetBool("Off"))
                    {
                        animator.SetTrigger("isOff");
                        animator.SetBool("Off", true);
                    }
                }

                return;
            }
            else
            {
                if (animator)
                {
                    if (animator.GetBool("Off"))
                    {
                        animator.SetBool("Off", false);
                    }
                }
            }

            if (animator)
            {
                //animator.SetBool("isShooting", shooting);
            }

            TrackMouse();

            CycleWeapons();

            DisabledWeaponCooldowns();

            ActiveTurret();
        }
    }

    private void ActiveTurret()
    {
        if (currentDroneWeapon.forcedCooldown)
        {
            shooting = false;
        }

        switch (currentDroneWeapon.shootingMode)
        {
            case DroneWeaponSO.ShootingMode.AUTO:
                locked = false;
                AutoTurret();
                break;

            case DroneWeaponSO.ShootingMode.LOCKON:
                LockOnTurret();
                break;

            case DroneWeaponSO.ShootingMode.MANUAL:
                locked = false;
                ManuelTurret();
                break;
        }
    }

    private void DisabledWeaponCooldowns()
    {
        foreach (DroneWeaponSO weapon in avaliableDroneWeaponList)
        {
            if (weapon != currentDroneWeapon)
            {
                ShootCooldown(weapon);
            }
        }
    }

    private void CycleWeapons()
    {
        if (switchTimer < maxSwitchTimer)
        {
            switchTimer += Time.deltaTime;
            return;
        }

        if (InputManager.Instance.HandleWeaponSwitch().WasPressedThisFrame())
        {
            int currentWeaponIndex = avaliableDroneWeaponList.IndexOf(currentDroneWeapon);

            SwitchWeapon(avaliableDroneWeaponList[(currentWeaponIndex + 1) % avaliableDroneWeaponList.Count]);
        }

        if (Input.GetKeyDown("1") && avaliableDroneWeaponList.Count >= 1)
        {
            SwitchWeapon(avaliableDroneWeaponList[0]);
        }
        else if (Input.GetKeyDown("2") && avaliableDroneWeaponList.Count >= 2)
        {
            if (avaliableDroneWeaponList[1].gunColour == DroneWeaponSO.GunColour.YELLOW)
            {
                SwitchWeapon(avaliableDroneWeaponList[1]);
            }
        }
        else if (Input.GetKeyDown("3") && avaliableDroneWeaponList.Count >= 2)
        {
            if (avaliableDroneWeaponList[1].gunColour == DroneWeaponSO.GunColour.YELLOW && avaliableDroneWeaponList.Count >= 3)
            {
                SwitchWeapon(avaliableDroneWeaponList[2]);
            }
            else if (avaliableDroneWeaponList[1].gunColour == DroneWeaponSO.GunColour.RED)
            {
                SwitchWeapon(avaliableDroneWeaponList[1]);
            }
        }
    }

    private void ManuelTurret()
    {
        if (InputManager.Instance.HandleAttackInput().WasPressedThisFrame() && !currentDroneWeapon.forcedCooldown)
        {
            Shoot();
        }
        else if (InputManager.Instance.HandleAttackInput().WasReleasedThisFrame())
        {
            shooting = false;
        }
        else
        {
            if (!shooting)
            {
                ShootCooldown(currentDroneWeapon);
            }
        }
    }

    private void AutoTurret()
    {
        shotTimer += Time.deltaTime;

        if (InputManager.Instance.HandleAttackInput().IsPressed() && shotTimer >= currentDroneWeapon.fireRate && !currentDroneWeapon.forcedCooldown)
        {
            shooting = true;

            Shoot();

            shotTimer = 0;
        }
        else if (InputManager.Instance.HandleAttackInput().WasReleasedThisFrame())
        {
            shooting = false;
        }
        else
        {
            if (!shooting)
            {
                ShootCooldown(currentDroneWeapon);
            }
        }
    }

    private void LockOnTurret()
    {
        if (InputManager.Instance.HandleAttackInput().WasPressedThisFrame() && !currentDroneWeapon.forcedCooldown)
        {
            shooting = true;

            if(locked)
            {
                locked = false;
            }
            else
            {
                locked = true;
                aimPos = mousePosition;
            }
        }
        else if (InputManager.Instance.HandleAttackInput().WasReleasedThisFrame())
        {
            shooting = false;
        }

        if (locked && !currentDroneWeapon.forcedCooldown)
        {
            shotTimer += Time.deltaTime;

            if (shotTimer >= currentDroneWeapon.fireRate)
            {
                Shoot();

                shotTimer = 0;
            }
        }
        else
        {
            if (locked && currentDroneWeapon.forcedCooldown)
            {
                locked = false;
            }

            if (!shooting)
            {
                ShootCooldown(currentDroneWeapon);
            }
        }
    }

    private void TrackMouse()
    {
        mousePosition = InputManager.Instance.GetCrossHairWorldPosition();

        float angle = 0;

        if (locked)
        {
            angle = LimitTurretMovement(aimPos);
        }
        else
        {
            angle = LimitTurretMovement(mousePosition);
        }

        angle -= 90;

        float angleInverse = 180 - currentDroneWeapon.trackMouseAngle;

        angle = Mathf.Clamp(angle, -180 + angleInverse / 2, -angleInverse / 2);

        transform.localRotation = Quaternion.AngleAxis(angle, Vector3.forward);
    }

    private float LimitTurretMovement(Vector2 pos)
    {
        float angle = 0;

        if (pos.x > droneTransform.position.x)
        {
            droneTransform.localEulerAngles = new Vector3(0, 0, 0);
            angle = Mathf.Atan2(pos.y - transform.position.y, pos.x - transform.position.x) * Mathf.Rad2Deg;
        }
        else if (pos.x < droneTransform.position.x)
        {
            droneTransform.localEulerAngles = new Vector3(0, 180, 0);
            angle = Mathf.Atan2(pos.y - transform.position.y, -(pos.x - transform.position.x)) * Mathf.Rad2Deg;
        }

        return angle;
    }

    private void Shoot()
    {
        if (currentDroneWeapon.forcedCooldown) return;

        if (!currentDroneWeapon.bulletPrefab) {Debug.LogError("Bullet Prefab is null."); return; }
      
        if (currentDroneWeapon.currentHeat >= currentDroneWeapon.overheatLimit) return; // Force cooldown

        if (animator) animator.SetTrigger("isShooting");

        for (int i = 0; i < shootPoints.Count; i++)
        {
            Quaternion rotation = shootPoints[i].rotation;

            int oddPro = 0;
            int evenPro = 0;

            for (int j = 0; j < currentDroneWeapon.bulletPerShot; j++)
            {
                currentDroneWeapon.currentHeat += currentDroneWeapon.heatPerShot;

                if (currentDroneWeapon.currentHeat >= currentDroneWeapon.overheatLimit)
                {
                    currentDroneWeapon.forcedCooldown = true;
                    currentDroneWeapon.currentHeat = currentDroneWeapon.overheatLimit;
                }

                UI_Manager.Instance.UpdatePlayerChargeUI(GetNormalizedHeat());

                Transform bulletTransfrom = Instantiate(currentDroneWeapon.bulletPrefab, shootPoints[i].position, rotation).transform;

                if (bulletTransfrom.TryGetComponent<Bullet>(out Bullet bullet))
                {
                    bullet.SetUp(bulletTransfrom.up ,currentDroneWeapon.GetSpeed(), currentDroneWeapon.bulletTravelTime);
                }

                if (j % 2 == 0) //Even
                {
                    evenPro += currentDroneWeapon.bulletSpread;
                    rotation = Quaternion.Euler(0, 0, evenPro) * shootPoints[i].rotation;
                }
                else //Odd
                {
                    oddPro -= currentDroneWeapon.bulletSpread;
                    rotation = Quaternion.Euler(0, 0, oddPro) * shootPoints[i].rotation;
                }
            }
        }
    }

    private void ShootCooldown(DroneWeaponSO weapon)
    {
        if (weapon == null) return;

        weapon.currentHeat = Mathf.MoveTowards(weapon.currentHeat, 0, Time.deltaTime * weapon.GetCooldownRate());

        if (weapon.currentHeat <= 0)
        {
            weapon.currentHeat = 0;

            if (weapon.forcedCooldown)
            {
                weapon.forcedCooldown = false;
            }
        }

        if (currentDroneWeapon == weapon)
        {
            UI_Manager.Instance.UpdatePlayerChargeUI(GetNormalizedHeat(), weapon.forcedCooldown);
        }
    }

    public void SwitchWeapon(DroneWeaponSO newWeapon)
    {
        if (newWeapon == null) return;

        if (!avaliableDroneWeaponList.Contains(newWeapon))
        {
            if (avaliableDroneWeaponList.Count >= 2)
            {
                if (avaliableDroneWeaponList[1].gunColour == DroneWeaponSO.GunColour.RED)
                {
                    avaliableDroneWeaponList.Insert(1, newWeapon);
                }
                else
                {
                    avaliableDroneWeaponList.Add(newWeapon);
                }
            }
            else
            {
                avaliableDroneWeaponList.Add(newWeapon);
            }
        }

        switchTimer = 0;
        ActivateWeapon(newWeapon);
    }

    private void ActivateWeapon(DroneWeaponSO newWeapon)
    {
        currentDroneWeapon = newWeapon;
        UI_Manager.Instance.UpdatePlayerChargeUI(GetNormalizedHeat());
        SetTurretColour();
    }

    private void SetTurretColour()
    {
        if (currentDroneWeapon.gunColour == DroneWeaponSO.GunColour.RED)
        {
            animator.SetBool("Red", true);
            animator.SetBool("Yellow", false);
            animator.SetBool("Blue", false);
            UI_Manager.Instance.UpdateWeaponUI(3);
        }
        else if (currentDroneWeapon.gunColour == DroneWeaponSO.GunColour.YELLOW)
        {
            animator.SetBool("Red", false);
            animator.SetBool("Yellow", true);
            animator.SetBool("Blue", false);
            UI_Manager.Instance.UpdateWeaponUI(2);
        }
        else if (currentDroneWeapon.gunColour == DroneWeaponSO.GunColour.BLUE)
        {
            animator.SetBool("Red", false);
            animator.SetBool("Yellow", false);
            animator.SetBool("Blue", true);
            UI_Manager.Instance.UpdateWeaponUI(1);
        }
    }

    private void SetFirePoints(GameObject go)
    {
        shootPoints.Add(go.transform.GetChild(0));
    }

    public void ToggleTurret(bool state)
    {
        activated = state;
    }

    public float GetNormalizedHeat()
    {
        return currentDroneWeapon.currentHeat / currentDroneWeapon.overheatLimit;
    }

    public float GetCurrentHeat()
    {
        return currentDroneWeapon.currentHeat;
    }

    public float GetHeatLimit()
    {
        return currentDroneWeapon.overheatLimit;
    }

}