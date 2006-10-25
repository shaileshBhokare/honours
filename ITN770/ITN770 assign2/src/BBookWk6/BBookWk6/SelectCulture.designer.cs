namespace BirthdayBook
{
    partial class SelectCulture
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.radNativeName = new System.Windows.Forms.RadioButton();
            this.radEnglishName = new System.Windows.Forms.RadioButton();
            this.radDispalyName = new System.Windows.Forms.RadioButton();
            this.chbShowAllCultures = new System.Windows.Forms.CheckBox();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.cmdOK = new System.Windows.Forms.Button();
            this.cboUICulture = new System.Windows.Forms.ComboBox();
            this.cboCulture = new System.Windows.Forms.ComboBox();
            this.lblUICulture = new System.Windows.Forms.Label();
            this.lblCulture = new System.Windows.Forms.Label();
            this.lblSelectUICul = new System.Windows.Forms.Label();
            this.lblSelectCul = new System.Windows.Forms.Label();
            this.lblInstalledUICul = new System.Windows.Forms.Label();
            this.lblInstalledCul = new System.Windows.Forms.Label();
            this.lblIntroduction = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // radNativeName
            // 
            this.radNativeName.Location = new System.Drawing.Point(197, 281);
            this.radNativeName.Name = "radNativeName";
            this.radNativeName.Size = new System.Drawing.Size(156, 24);
            this.radNativeName.TabIndex = 29;
            this.radNativeName.Text = "[NATIVE NAME]";
            this.radNativeName.CheckedChanged += new System.EventHandler(this.optNativeName_CheckedChanged);
            // 
            // radEnglishName
            // 
            this.radEnglishName.Location = new System.Drawing.Point(197, 256);
            this.radEnglishName.Name = "radEnglishName";
            this.radEnglishName.Size = new System.Drawing.Size(156, 24);
            this.radEnglishName.TabIndex = 28;
            this.radEnglishName.Text = "[ENGLISH NAME]";
            this.radEnglishName.CheckedChanged += new System.EventHandler(this.optEnglishName_CheckedChanged);
            // 
            // radDispalyName
            // 
            this.radDispalyName.Checked = true;
            this.radDispalyName.Location = new System.Drawing.Point(197, 226);
            this.radDispalyName.Name = "radDispalyName";
            this.radDispalyName.Size = new System.Drawing.Size(156, 24);
            this.radDispalyName.TabIndex = 27;
            this.radDispalyName.TabStop = true;
            this.radDispalyName.Text = "[DISPLAY NAME]";
            this.radDispalyName.CheckedChanged += new System.EventHandler(this.optDisplayName_CheckedChanged);
            // 
            // chbShowAllCultures
            // 
            this.chbShowAllCultures.Location = new System.Drawing.Point(16, 226);
            this.chbShowAllCultures.Name = "chbShowAllCultures";
            this.chbShowAllCultures.Size = new System.Drawing.Size(175, 24);
            this.chbShowAllCultures.TabIndex = 26;
            this.chbShowAllCultures.Text = "[SHOW ALL CULTURES]";
            this.chbShowAllCultures.CheckedChanged += new System.EventHandler(this.chkShowAll_CheckedChanged);
            // 
            // cmdCancel
            // 
            this.cmdCancel.Location = new System.Drawing.Point(384, 257);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(136, 23);
            this.cmdCancel.TabIndex = 25;
            this.cmdCancel.Text = "[CANCEL]";
            this.cmdCancel.Click += new System.EventHandler(this.cmdExit_Click);
            // 
            // cmdOK
            // 
            this.cmdOK.Location = new System.Drawing.Point(384, 226);
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.Size = new System.Drawing.Size(136, 23);
            this.cmdOK.TabIndex = 24;
            this.cmdOK.Text = "[OK]";
            this.cmdOK.Click += new System.EventHandler(this.cmdStart_Click);
            // 
            // cboUICulture
            // 
            this.cboUICulture.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboUICulture.Items.AddRange(new object[] {
            "de-DE ",
            "en-AU (no resources available)",
            "en-US",
            "jp-JP (no resources available)"});
            this.cboUICulture.Location = new System.Drawing.Point(280, 186);
            this.cboUICulture.Name = "cboUICulture";
            this.cboUICulture.Size = new System.Drawing.Size(240, 21);
            this.cboUICulture.Sorted = true;
            this.cboUICulture.TabIndex = 19;
            // 
            // cboCulture
            // 
            this.cboCulture.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboCulture.Items.AddRange(new object[] {
            "de-DE ",
            "en-AU (no resources available)",
            "en-US",
            "jp-JP (no resources available)"});
            this.cboCulture.Location = new System.Drawing.Point(16, 186);
            this.cboCulture.Name = "cboCulture";
            this.cboCulture.Size = new System.Drawing.Size(240, 21);
            this.cboCulture.Sorted = true;
            this.cboCulture.TabIndex = 18;
            // 
            // lblUICulture
            // 
            this.lblUICulture.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblUICulture.Location = new System.Drawing.Point(280, 138);
            this.lblUICulture.Name = "lblUICulture";
            this.lblUICulture.Size = new System.Drawing.Size(240, 21);
            this.lblUICulture.TabIndex = 17;
            // 
            // lblCulture
            // 
            this.lblCulture.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblCulture.Location = new System.Drawing.Point(16, 138);
            this.lblCulture.Name = "lblCulture";
            this.lblCulture.Size = new System.Drawing.Size(240, 23);
            this.lblCulture.TabIndex = 16;
            // 
            // lblSelectUICul
            // 
            this.lblSelectUICul.Location = new System.Drawing.Point(280, 170);
            this.lblSelectUICul.Name = "lblSelectUICul";
            this.lblSelectUICul.Size = new System.Drawing.Size(240, 21);
            this.lblSelectUICul.TabIndex = 23;
            this.lblSelectUICul.Text = "[SELECT UI CULTURE]";
            // 
            // lblSelectCul
            // 
            this.lblSelectCul.Location = new System.Drawing.Point(16, 170);
            this.lblSelectCul.Name = "lblSelectCul";
            this.lblSelectCul.Size = new System.Drawing.Size(240, 23);
            this.lblSelectCul.TabIndex = 22;
            this.lblSelectCul.Text = "[SELECT CULTURE]";
            // 
            // lblInstalledUICul
            // 
            this.lblInstalledUICul.Location = new System.Drawing.Point(280, 122);
            this.lblInstalledUICul.Name = "lblInstalledUICul";
            this.lblInstalledUICul.Size = new System.Drawing.Size(240, 21);
            this.lblInstalledUICul.TabIndex = 21;
            this.lblInstalledUICul.Text = "[INSTALLED UI CULTURE]";
            // 
            // lblInstalledCul
            // 
            this.lblInstalledCul.Location = new System.Drawing.Point(16, 122);
            this.lblInstalledCul.Name = "lblInstalledCul";
            this.lblInstalledCul.Size = new System.Drawing.Size(240, 23);
            this.lblInstalledCul.TabIndex = 20;
            this.lblInstalledCul.Text = "[INSTALLED CULTURE]";
            // 
            // lblIntroduction
            // 
            this.lblIntroduction.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblIntroduction.Location = new System.Drawing.Point(24, 18);
            this.lblIntroduction.Name = "lblIntroduction";
            this.lblIntroduction.Size = new System.Drawing.Size(496, 104);
            this.lblIntroduction.TabIndex = 15;
            this.lblIntroduction.Text = "[THIS APPLICATION STARTUP FORM ALLOWS YOU TO RESET THE CULTURE AND USER INTERFACE" +
                " CULTURE BEFORE STARTING AN APPLICATION. USE EXIT TO TERMINATE IMMEDIATELY]";
            // 
            // SelectCulture
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(536, 317);
            this.Controls.Add(this.radNativeName);
            this.Controls.Add(this.radEnglishName);
            this.Controls.Add(this.radDispalyName);
            this.Controls.Add(this.chbShowAllCultures);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdOK);
            this.Controls.Add(this.cboUICulture);
            this.Controls.Add(this.cboCulture);
            this.Controls.Add(this.lblUICulture);
            this.Controls.Add(this.lblCulture);
            this.Controls.Add(this.lblSelectUICul);
            this.Controls.Add(this.lblSelectCul);
            this.Controls.Add(this.lblInstalledUICul);
            this.Controls.Add(this.lblInstalledCul);
            this.Controls.Add(this.lblIntroduction);
            this.Name = "SelectCulture";
            this.Text = "[SELECT CULTURE]";
            this.Load += new System.EventHandler(this.SelectCulture_Load);
            this.ResumeLayout(false);

        }

        #endregion

        internal System.Windows.Forms.RadioButton radNativeName;
        internal System.Windows.Forms.RadioButton radEnglishName;
        internal System.Windows.Forms.RadioButton radDispalyName;
        internal System.Windows.Forms.CheckBox chbShowAllCultures;
        internal System.Windows.Forms.Button cmdCancel;
        internal System.Windows.Forms.Button cmdOK;
        internal System.Windows.Forms.ComboBox cboUICulture;
        internal System.Windows.Forms.ComboBox cboCulture;
        internal System.Windows.Forms.Label lblUICulture;
        internal System.Windows.Forms.Label lblCulture;
        internal System.Windows.Forms.Label lblSelectUICul;
        internal System.Windows.Forms.Label lblSelectCul;
        internal System.Windows.Forms.Label lblInstalledUICul;
        internal System.Windows.Forms.Label lblInstalledCul;
        internal System.Windows.Forms.Label lblIntroduction;

    }
}