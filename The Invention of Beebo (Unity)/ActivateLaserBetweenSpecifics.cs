using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Laser), typeof(MovingPlatform))]
public class ActivateLaserBetweenSpecifics : MonoBehaviour
{
    private Laser laser;
    private MovingPlatform mP;

    private bool toggled = false;

    private void Start()
    {
        laser = GetComponent<Laser>();
        mP = GetComponent<MovingPlatform>();

        laser.Toggle(false);
    }

    private void Update()
    {
        if (!mP.destroyOnTarget)
        {
            if (!toggled)
            {
                laser.Toggle(true);
                toggled = true;
            }

            return;
        }

        if (mP.i > 1 && mP.i < mP.points.Length-1 && !toggled)
        {
            laser.Toggle(true);
            toggled = true;
        }
        else if ((mP.i <= 1 || mP.i >= mP.points.Length - 1) && toggled)
        {
            laser.Toggle(false);
            toggled = false;
        }
    }
}
