using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Globalization;
using System.Threading;

namespace BirthdayBook
{
    public partial class SelectCulture : Form
    {
        public SelectCulture()
        {
            InitializeComponent();
        }

        private void LoadCultures() {
            CultureInfo[] list; 
            String name; 

            //Get a list of cultures - either all or installed
            if (chkShowAll.Checked) { 
                list = CultureInfo.GetCultures(CultureTypes.AllCultures); 
            } else { 
                list = CultureInfo.GetCultures(CultureTypes.InstalledWin32Cultures);
            }
            
            //Fill the combo boxes
            cboCulture.Items.Clear();
            cboUICulture.Items.Clear();

            //There is a name and a display name
            foreach (CultureInfo ci in list) { 
                if (optEnglishName.Checked) {
                    name = ci.EnglishName;
                } else if (optNativeName.Checked) {
                    name = ci.NativeName;
                } else {
                    name = ci.DisplayName;
                }

                //bind the names together and show for 
                //both culture and UIculture
                name = ci.Name + " " + name;
                cboUICulture.Items.Add(name);
                cboCulture.Items.Add(name);
            }

            cboCulture.SelectedIndex = 0;
            cboUICulture.SelectedIndex = 0;
        }
        
        //Event handlers all reload cultures upon check changed event. 
        //Note that the display name is selected by default
        private void chkShowAll_CheckedChanged(object sender, EventArgs e)
        {
            LoadCultures();
        }

        private void optDisplayName_CheckedChanged(object sender, EventArgs e)
        {
            LoadCultures();
        }

        private void optEnglishName_CheckedChanged(object sender, EventArgs e)
        {
            LoadCultures();
        }

        private void optNativeName_CheckedChanged(object sender, EventArgs e)
        {
            LoadCultures();
        }

        /// <summary>
        /// Event Handler to set the CUlture and UICulture, leading into 
        /// the display of the form
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdStart_Click(object sender, EventArgs e)
        {
            //Auxiliary variables 
            int spaceIndex=0;

            //Process the dialog, checking for aborts  
            this.DialogResult = DialogResult.OK;

            //We need to grab the string-based culture identifier
            String cultureID =  cboCulture.Text;
            if (cultureID.Length!=0) { 
                spaceIndex = cultureID.IndexOf(" ");
                if (spaceIndex >= 0) {
                    cultureID = cultureID.Substring(0, spaceIndex);
                }

                //Now we have it, so we set the CurrentCulture
                //Note that we need the thread class for this
                Thread.CurrentThread.CurrentCulture = new CultureInfo(cultureID);
            }

            //The same process, this time for the UICulture
            cultureID = cboUICulture.Text;
            if (cultureID.Length!=0) { 
                spaceIndex = cultureID.IndexOf(" ");
                if (spaceIndex >= 0) {
                    cultureID = cultureID.Substring(0, spaceIndex);
                }

                Thread.CurrentThread.CurrentUICulture = new CultureInfo(cultureID);
            }
            this.Close();

        }

        /// <summary>
        /// Event Handler kills the form, returning a dialog abort
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdExit_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Abort;
            this.Close();
        }

        /// <summary>
        /// Load Event Handler is responsible for display of culture 
        /// names for the thread, and loading of initial cultures
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SelectCulture_Load(object sender, EventArgs e)
        {
            this.AcceptButton = cmdOK;
            this.CancelButton = cmdCancel;

            //Load the culture names
            LoadCultures();

            //Display the current cultures of the thread.
            //These are not subject to the selection made 
            lblCulture.Text = Thread.CurrentThread.CurrentCulture.EnglishName;
            lblUICulture.Text = Thread.CurrentThread.CurrentUICulture.EnglishName;
        }
    }
}