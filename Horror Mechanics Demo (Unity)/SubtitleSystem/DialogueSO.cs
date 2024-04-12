using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "My Assets/Dialogue ")]
public class DialogueSO : ScriptableObject
{
    public string sentence = "This is example text.";
    public AudioClip sentenceAudio;

    public float textAppendSpeed = 0.1f;
}
