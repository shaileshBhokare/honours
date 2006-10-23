using System;
using System.Resources;
using System.Text;
using System.Collections;
using System.Reflection;
using System.Windows.Forms;

namespace BirthdayBook
{
    public class StringResManager : ResourceManager
    {

        private bool showError;

        public StringResManager(string baseName, Assembly assembly): 
            base(baseName, assembly) {
            this.showError = true;
        }

        public bool ShowError
        {
            set { this.showError = value; }
            get { return this.showError; }
        }

        //Get a string resource. If we fail, then build a default
        //resource value from the key. Method overrides the standard GetString 
        //checks for null strings and handles exceptions... 

        public override string GetString(String name) {
            string result; 
            try { 
              //Hopefully valid, but check
              result = base.GetString(name);            
              if (result == null) {
                  throw new MissingManifestResourceException();
              }
            } catch (Exception ex) { 
              //For whatever reason, we can't find the resource
              result = String.Format("[?{0}?]", name);
              //Display the result?
              if (this.showError) {
                  string str = String.Format("Failed to find resource: {0} \nKeep reporting errors?", name);
                  if (MessageBox.Show(str,"BirthdayBook",MessageBoxButtons.YesNo,MessageBoxIcon.Stop)==DialogResult.No) {
                      this.showError=false;
                  }
              } 
            } 
            return result;
        }
    }
}
