namespace BirthdayBook
{
    partial class frmSearch
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtInput = new System.Windows.Forms.TextBox();
            this.cbxCulture = new System.Windows.Forms.ComboBox();
            this.btnUpdate = new System.Windows.Forms.Button();
            this.lbxResult = new System.Windows.Forms.ListBox();
            this.btnSortKey = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(98, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "[SEARCHSTRING]";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 70);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "[CULTURE]";
            // 
            // txtInput
            // 
            this.txtInput.Location = new System.Drawing.Point(141, 29);
            this.txtInput.Name = "txtInput";
            this.txtInput.Size = new System.Drawing.Size(199, 20);
            this.txtInput.TabIndex = 2;
            // 
            // cbxCulture
            // 
            this.cbxCulture.FormattingEnabled = true;
            this.cbxCulture.Location = new System.Drawing.Point(141, 67);
            this.cbxCulture.Name = "cbxCulture";
            this.cbxCulture.Size = new System.Drawing.Size(199, 21);
            this.cbxCulture.TabIndex = 3;
            // 
            // btnUpdate
            // 
            this.btnUpdate.Location = new System.Drawing.Point(89, 119);
            this.btnUpdate.Name = "btnUpdate";
            this.btnUpdate.Size = new System.Drawing.Size(75, 23);
            this.btnUpdate.TabIndex = 4;
            this.btnUpdate.Text = "[UPDATE]";
            this.btnUpdate.UseVisualStyleBackColor = true;
            this.btnUpdate.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // lbxResult
            // 
            this.lbxResult.FormattingEnabled = true;
            this.lbxResult.Location = new System.Drawing.Point(12, 173);
            this.lbxResult.Name = "lbxResult";
            this.lbxResult.Size = new System.Drawing.Size(328, 134);
            this.lbxResult.TabIndex = 5;
            // 
            // btnSortKey
            // 
            this.btnSortKey.Location = new System.Drawing.Point(205, 119);
            this.btnSortKey.Name = "btnSortKey";
            this.btnSortKey.Size = new System.Drawing.Size(135, 23);
            this.btnSortKey.TabIndex = 6;
            this.btnSortKey.Text = "[USING SORT KEY]";
            this.btnSortKey.UseVisualStyleBackColor = true;
            this.btnSortKey.Click += new System.EventHandler(this.btnSortKey_Click);
            // 
            // frmSearch
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(365, 345);
            this.Controls.Add(this.btnSortKey);
            this.Controls.Add(this.lbxResult);
            this.Controls.Add(this.btnUpdate);
            this.Controls.Add(this.cbxCulture);
            this.Controls.Add(this.txtInput);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "frmSearch";
            this.Text = "[SEARCH]";
            this.Load += new System.EventHandler(this.frmSearch_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtInput;
        private System.Windows.Forms.ComboBox cbxCulture;
        private System.Windows.Forms.Button btnUpdate;
        private System.Windows.Forms.ListBox lbxResult;
        private System.Windows.Forms.Button btnSortKey;
    }
}