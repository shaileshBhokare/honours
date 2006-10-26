using System;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

// Making this code re-entrant and adding error checking is left as an exercise 
// for the reader.
namespace BirthdayBook
{
    public class MessageBoxEx
    {
        /*public static void Main(string[] args)
        {
            MessageBoxEx.Show(
                "Do you want to save before exiting?",
                "Save",
                MessageBoxIcon.Question,
                MessageBoxButtons.YesNoCancel,
                "Save", "Don't Save", "Cancel");
        }*/

        // Hook function delegate; for SetWindowsHookEx.
        private delegate int HookProc(int code, IntPtr wParam, IntPtr lParam);

        // Windows API Declarations
        private const int WH_CBT = 5; // Windows Hook type: "computer-based training"
        private const int HCBT_CREATEWND = 3; // CBT message type: window creation
        private const int HCBT_ACTIVATE = 5; // CBT message type: window activation

        [DllImport("user32.dll")]
        private static extern IntPtr SetWindowsHookEx(int code, HookProc func,
                                                      IntPtr hInstance, int threadID);

        [DllImport("user32.dll")]
        private static extern int UnhookWindowsHookEx(IntPtr hHook);

        [DllImport("user32.dll")]
        private static extern int CallNextHookEx(IntPtr hHook, int code,
                                                 IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll")]
        private static extern int GetClassName(IntPtr hwnd,
                                               StringBuilder lpClassName,
                                               int nMaxCount);

        [DllImport("user32.dll")]
        private static extern void SetWindowText(IntPtr hWnd, string text);

        // State variables to keep track of things while the filter code is
        // running.
        private static IntPtr hookHandle;
        private static int currentButton;
        private static string[] buttonTexts;
        private static IntPtr[] buttonHandles;

        public static void Show(string text, string caption, MessageBoxIcon icon,
            MessageBoxButtons returnValues,params string[] buttons)
        {
            HookProc filter = new HookProc(FilterProc);
            buttonTexts = buttons;
            currentButton = 0;
            buttonHandles = new IntPtr[buttons.Length];

            // Install the windows hook
            hookHandle = SetWindowsHookEx(WH_CBT, filter, IntPtr.Zero,
                (int)AppDomain.GetCurrentThreadId());

            // Display the message box.
            MessageBox.Show(text, caption, returnValues, icon);

            // Uninstall the windows hook
            UnhookWindowsHookEx(hookHandle);
        }

        public static void Show(string text, string caption, MessageBoxIcon icon,
            MessageBoxButtons returnValues,MessageBoxOptions option, params string[] buttons)
        {
            HookProc filter = new HookProc(FilterProc);
            buttonTexts = buttons;
            currentButton = 0;
            buttonHandles = new IntPtr[buttons.Length];

            // Install the windows hook
            hookHandle = SetWindowsHookEx(WH_CBT, filter, IntPtr.Zero,
                (int)AppDomain.GetCurrentThreadId());

            // Display the message box.
            MessageBox.Show(text, caption, returnValues, icon, MessageBoxDefaultButton.Button1,option);

            // Uninstall the windows hook
            UnhookWindowsHookEx(hookHandle);
        }

        private static int FilterProc(int code, IntPtr wParam, IntPtr lParam)
        {
            // Per MSDN, don't attempt to handle negative codes.
            if (code < 0)
                return (CallNextHookEx(hookHandle, code, wParam, lParam));

            if (code == HCBT_CREATEWND)
            {
                // A control is being created.  Check to see if this is one
                // of the buttons.
                StringBuilder ctlClass = new StringBuilder();
                ctlClass.Capacity = 40;
                GetClassName(wParam, ctlClass, 40);
                // wParam is the handle to the control being created.

                if (ctlClass.ToString() == "Button")
                {
                    // It is a button.  Store away the handle to this control so 
                    // that we can, at window activation time, set the text as 
                    // we please.
                    buttonHandles[currentButton++] = wParam;
                }
            }
            else if (code == HCBT_ACTIVATE)
            {
                // The message box window is being displayed.  All of the internal
                // initialization code is done, so we can mangle the message box
                // as much as we'd like now.
                for (int i = 0; i < buttonHandles.Length; i++)
                    SetWindowText(buttonHandles[i], buttonTexts[i]);
            }

            // Give the next hook in the chain a chance to filter.
            return (CallNextHookEx(hookHandle, code, wParam, lParam));
        }
    }
}