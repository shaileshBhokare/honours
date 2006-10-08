namespace BirthdayBook
{
    partial class frmSummary
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
            this.btnClose = new System.Windows.Forms.Button();
            this.gbxInfo = new System.Windows.Forms.GroupBox();
            this.lblRowNumberValue = new System.Windows.Forms.Label();
            this.lblColumnNameValue = new System.Windows.Forms.Label();
            this.lblColumnNameText = new System.Windows.Forms.Label();
            this.lblColumnNumberValue = new System.Windows.Forms.Label();
            this.lblRowNumberText = new System.Windows.Forms.Label();
            this.lblColumnNumText = new System.Windows.Forms.Label();
            this.gbxInfo.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(224, 164);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 2;
            this.btnClose.Text = "[CLOSE]";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // gbxInfo
            // 
            this.gbxInfo.Controls.Add(this.lblRowNumberValue);
            this.gbxInfo.Controls.Add(this.lblColumnNameValue);
            this.gbxInfo.Controls.Add(this.lblColumnNameText);
            this.gbxInfo.Controls.Add(this.lblColumnNumberValue);
            this.gbxInfo.Controls.Add(this.lblRowNumberText);
            this.gbxInfo.Controls.Add(this.lblColumnNumText);
            this.gbxInfo.Location = new System.Drawing.Point(12, 12);
            this.gbxInfo.Name = "gbxInfo";
            this.gbxInfo.Size = new System.Drawing.Size(331, 146);
            this.gbxInfo.TabIndex = 6;
            this.gbxInfo.TabStop = false;
            this.gbxInfo.Text = "[INFORMATION]";
            // 
            // lblRowNumberValue
            // 
            this.lblRowNumberValue.AutoSize = true;
            this.lblRowNumberValue.Location = new System.Drawing.Point(168, 106);
            this.lblRowNumberValue.Name = "lblRowNumberValue";
            this.lblRowNumberValue.Size = new System.Drawing.Size(0, 13);
            this.lblRowNumberValue.TabIndex = 11;
            // 
            // lblColumnNameValue
            // 
            this.lblColumnNameValue.AutoSize = true;
            this.lblColumnNameValue.Location = new System.Drawing.Point(168, 70);
            this.lblColumnNameValue.Name = "lblColumnNameValue";
            this.lblColumnNameValue.Size = new System.Drawing.Size(0, 13);
            this.lblColumnNameValue.TabIndex = 10;
            // 
            // lblColumnNameText
            // 
            this.lblColumnNameText.AutoSize = true;
            this.lblColumnNameText.Location = new System.Drawing.Point(20, 70);
            this.lblColumnNameText.Name = "lblColumnNameText";
            this.lblColumnNameText.Size = new System.Drawing.Size(93, 13);
            this.lblColumnNameText.TabIndex = 9;
            this.lblColumnNameText.Text = "[COLUMN NAME]";
            // 
            // lblColumnNumberValue
            // 
            this.lblColumnNumberValue.AutoSize = true;
            this.lblColumnNumberValue.Location = new System.Drawing.Point(168, 36);
            this.lblColumnNumberValue.Name = "lblColumnNumberValue";
            this.lblColumnNumberValue.Size = new System.Drawing.Size(0, 13);
            this.lblColumnNumberValue.TabIndex = 8;
            // 
            // lblRowNumberText
            // 
            this.lblRowNumberText.AutoSize = true;
            this.lblRowNumberText.Location = new System.Drawing.Point(20, 106);
            this.lblRowNumberText.Name = "lblRowNumberText";
            this.lblRowNumberText.Size = new System.Drawing.Size(90, 13);
            this.lblRowNumberText.TabIndex = 7;
            this.lblRowNumberText.Text = "[ROW NUMBER]";
            // 
            // lblColumnNumText
            // 
            this.lblColumnNumText.AutoSize = true;
            this.lblColumnNumText.Location = new System.Drawing.Point(20, 36);
            this.lblColumnNumText.Name = "lblColumnNumText";
            this.lblColumnNumText.Size = new System.Drawing.Size(109, 13);
            this.lblColumnNumText.TabIndex = 6;
            this.lblColumnNumText.Text = "[COLUMN NUMBER]";
            // 
            // frmSummary
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(355, 199);
            this.Controls.Add(this.gbxInfo);
            this.Controls.Add(this.btnClose);
            this.Name = "frmSummary";
            this.Text = "[SUMMARY]";
            this.Load += new System.EventHandler(this.frmSummary_Load);
            this.gbxInfo.ResumeLayout(false);
            this.gbxInfo.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.GroupBox gbxInfo;
        private System.Windows.Forms.Label lblRowNumberValue;
        private System.Windows.Forms.Label lblColumnNameValue;
        private System.Windows.Forms.Label lblColumnNameText;
        private System.Windows.Forms.Label lblColumnNumberValue;
        private System.Windows.Forms.Label lblRowNumberText;
        private System.Windows.Forms.Label lblColumnNumText;
    }
}