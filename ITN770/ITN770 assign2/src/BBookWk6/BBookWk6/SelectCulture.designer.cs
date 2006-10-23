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
            this.optNativeName = new System.Windows.Forms.RadioButton();
            this.optEnglishName = new System.Windows.Forms.RadioButton();
            this.optDisplayName = new System.Windows.Forms.RadioButton();
            this.chkShowAll = new System.Windows.Forms.CheckBox();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.cmdOK = new System.Windows.Forms.Button();
            this.cboUICulture = new System.Windows.Forms.ComboBox();
            this.cboCulture = new System.Windows.Forms.ComboBox();
            this.lblUICulture = new System.Windows.Forms.Label();
            this.lblCulture = new System.Windows.Forms.Label();
            this.Label4 = new System.Windows.Forms.Label();
            this.Label5 = new System.Windows.Forms.Label();
            this.Label3 = new System.Windows.Forms.Label();
            this.Label2 = new System.Windows.Forms.Label();
            this.Label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // optNativeName
            // 
            this.optNativeName.Location = new System.Drawing.Point(192, 274);
            this.optNativeName.Name = "optNativeName";
            this.optNativeName.Size = new System.Drawing.Size(104, 24);
            this.optNativeName.TabIndex = 29;
            this.optNativeName.Text = "Native Name";
            this.optNativeName.CheckedChanged += new System.EventHandler(this.optNativeName_CheckedChanged);
            // 
            // optEnglishName
            // 
            this.optEnglishName.Location = new System.Drawing.Point(192, 250);
            this.optEnglishName.Name = "optEnglishName";
            this.optEnglishName.Size = new System.Drawing.Size(104, 24);
            this.optEnglishName.TabIndex = 28;
            this.optEnglishName.Text = "English Name";
            this.optEnglishName.CheckedChanged += new System.EventHandler(this.optEnglishName_CheckedChanged);
            // 
            // optDisplayName
            // 
            this.optDisplayName.Checked = true;
            this.optDisplayName.Location = new System.Drawing.Point(192, 226);
            this.optDisplayName.Name = "optDisplayName";
            this.optDisplayName.Size = new System.Drawing.Size(104, 24);
            this.optDisplayName.TabIndex = 27;
            this.optDisplayName.TabStop = true;
            this.optDisplayName.Text = "Display Name";
            this.optDisplayName.CheckedChanged += new System.EventHandler(this.optDisplayName_CheckedChanged);
            // 
            // chkShowAll
            // 
            this.chkShowAll.Location = new System.Drawing.Point(16, 226);
            this.chkShowAll.Name = "chkShowAll";
            this.chkShowAll.Size = new System.Drawing.Size(152, 24);
            this.chkShowAll.TabIndex = 26;
            this.chkShowAll.Text = "Show All Cultures";
            this.chkShowAll.CheckedChanged += new System.EventHandler(this.chkShowAll_CheckedChanged);
            // 
            // cmdCancel
            // 
            this.cmdCancel.Location = new System.Drawing.Point(336, 258);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(136, 23);
            this.cmdCancel.TabIndex = 25;
            this.cmdCancel.Text = "Cancel";
            this.cmdCancel.Click += new System.EventHandler(this.cmdExit_Click);
            // 
            // cmdOK
            // 
            this.cmdOK.Location = new System.Drawing.Point(336, 226);
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.Size = new System.Drawing.Size(136, 23);
            this.cmdOK.TabIndex = 24;
            this.cmdOK.Text = "OK";
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
            // Label4
            // 
            this.Label4.Location = new System.Drawing.Point(280, 170);
            this.Label4.Name = "Label4";
            this.Label4.Size = new System.Drawing.Size(240, 21);
            this.Label4.TabIndex = 23;
            this.Label4.Text = "Select UI Culture";
            // 
            // Label5
            // 
            this.Label5.Location = new System.Drawing.Point(16, 170);
            this.Label5.Name = "Label5";
            this.Label5.Size = new System.Drawing.Size(240, 23);
            this.Label5.TabIndex = 22;
            this.Label5.Text = "Select Culture";
            // 
            // Label3
            // 
            this.Label3.Location = new System.Drawing.Point(280, 122);
            this.Label3.Name = "Label3";
            this.Label3.Size = new System.Drawing.Size(240, 21);
            this.Label3.TabIndex = 21;
            this.Label3.Text = "Installed UI Culture";
            // 
            // Label2
            // 
            this.Label2.Location = new System.Drawing.Point(16, 122);
            this.Label2.Name = "Label2";
            this.Label2.Size = new System.Drawing.Size(240, 23);
            this.Label2.TabIndex = 20;
            this.Label2.Text = "Installed Culture";
            // 
            // Label1
            // 
            this.Label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Label1.Location = new System.Drawing.Point(24, 18);
            this.Label1.Name = "Label1";
            this.Label1.Size = new System.Drawing.Size(496, 104);
            this.Label1.TabIndex = 15;
            this.Label1.Text = "This application startup form allows you to reset the Culture and User Interface " +
                "Culture before starting an application. Use Exit to terminate immediately.";
            // 
            // SelectCulture
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(536, 317);
            this.Controls.Add(this.optNativeName);
            this.Controls.Add(this.optEnglishName);
            this.Controls.Add(this.optDisplayName);
            this.Controls.Add(this.chkShowAll);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdOK);
            this.Controls.Add(this.cboUICulture);
            this.Controls.Add(this.cboCulture);
            this.Controls.Add(this.lblUICulture);
            this.Controls.Add(this.lblCulture);
            this.Controls.Add(this.Label4);
            this.Controls.Add(this.Label5);
            this.Controls.Add(this.Label3);
            this.Controls.Add(this.Label2);
            this.Controls.Add(this.Label1);
            this.Name = "SelectCulture";
            this.Text = "SelectCulture";
            this.Load += new System.EventHandler(this.SelectCulture_Load);
            this.ResumeLayout(false);

        }

        #endregion

        internal System.Windows.Forms.RadioButton optNativeName;
        internal System.Windows.Forms.RadioButton optEnglishName;
        internal System.Windows.Forms.RadioButton optDisplayName;
        internal System.Windows.Forms.CheckBox chkShowAll;
        internal System.Windows.Forms.Button cmdCancel;
        internal System.Windows.Forms.Button cmdOK;
        internal System.Windows.Forms.ComboBox cboUICulture;
        internal System.Windows.Forms.ComboBox cboCulture;
        internal System.Windows.Forms.Label lblUICulture;
        internal System.Windows.Forms.Label lblCulture;
        internal System.Windows.Forms.Label Label4;
        internal System.Windows.Forms.Label Label5;
        internal System.Windows.Forms.Label Label3;
        internal System.Windows.Forms.Label Label2;
        internal System.Windows.Forms.Label Label1;

    }
}