using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

/// <summary>
/// Handles both the subtitle system and the voice over that work in conjunction.
/// </summary>
public class SubtitleHandler : MonoBehaviour
{

    public static SubtitleHandler Instance { set; get; }

    private bool isReading = false;

    [SerializeField] private TextMeshProUGUI subtitleText;

    private AudioSource audioSource;

    private Queue<DialogueSO> dialogueQueue = new Queue<DialogueSO>();

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
        }
    }

    private void Start()
    {
        audioSource = GetComponent<AudioSource>();
    }

    private void Update()
    {
        if (!isReading && dialogueQueue.Count > 0)
        {
            SpeakAndSub(dialogueQueue.Dequeue());
        }
    }

    public void AddToSpeechQueue(DialogueSO dialogue)
    {
        dialogueQueue.Enqueue(dialogue);
    }

    private void SpeakAndSub(DialogueSO dialogue)
    {
        if (audioSource) audioSource.PlayOneShot(dialogue.sentenceAudio);
        WriteTextToSubtitle(dialogue);
    }

    #region Subtitle Functions
    private void WriteTextToSubtitle(DialogueSO dialogue)
    {
        subtitleText.text = "";
        StartCoroutine(AppendCharactersOverTime(dialogue));
    }

    private IEnumerator AppendCharactersOverTime(DialogueSO dialogue)
    {
        isReading = true;
        foreach (char letter in dialogue.sentence)
        {
            yield return new WaitForSeconds(dialogue.textAppendSpeed);

            subtitleText.text += letter;

            if (FullStop(letter))
            {
                yield return new WaitForSeconds(1f); // Allow time to read before refreshing.
                subtitleText.text = "";
            }
        }
        isReading = false;
    }

    private bool FullStop(char letter)
    {
        if (letter == '.')
        {
            return true;
        }

        return false;
    }
    #endregion

    public void ResetSubtitleReader()
    {
        dialogueQueue.Clear();
        isReading = false;
        subtitleText.text = "";
    }

}
