namespace HappyCookerGUI
{
    partial class Form1
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
            this.btn_LoadObj = new System.Windows.Forms.Button();
            this.pB_LoadObj = new System.Windows.Forms.ProgressBar();
            this.btn_CookBin = new System.Windows.Forms.Button();
            this.btn_CookConvex = new System.Windows.Forms.Button();
            this.btn_CookConcave = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btn_LoadObj
            // 
            this.btn_LoadObj.Location = new System.Drawing.Point(12, 12);
            this.btn_LoadObj.Name = "btn_LoadObj";
            this.btn_LoadObj.Size = new System.Drawing.Size(185, 31);
            this.btn_LoadObj.TabIndex = 0;
            this.btn_LoadObj.Text = "Load Obj";
            this.btn_LoadObj.UseVisualStyleBackColor = true;
            this.btn_LoadObj.Click += new System.EventHandler(this.btn_LoadObj_Click);
            // 
            // pB_LoadObj
            // 
            this.pB_LoadObj.Location = new System.Drawing.Point(12, 49);
            this.pB_LoadObj.Name = "pB_LoadObj";
            this.pB_LoadObj.Size = new System.Drawing.Size(185, 23);
            this.pB_LoadObj.Step = 1;
            this.pB_LoadObj.TabIndex = 1;
            // 
            // btn_CookBin
            // 
            this.btn_CookBin.Location = new System.Drawing.Point(12, 100);
            this.btn_CookBin.Name = "btn_CookBin";
            this.btn_CookBin.Size = new System.Drawing.Size(185, 34);
            this.btn_CookBin.TabIndex = 2;
            this.btn_CookBin.Text = "Cook Bin";
            this.btn_CookBin.UseVisualStyleBackColor = true;
            this.btn_CookBin.Click += new System.EventHandler(this.btn_CookBin_Click);
            // 
            // btn_CookConvex
            // 
            this.btn_CookConvex.Location = new System.Drawing.Point(12, 140);
            this.btn_CookConvex.Name = "btn_CookConvex";
            this.btn_CookConvex.Size = new System.Drawing.Size(185, 34);
            this.btn_CookConvex.TabIndex = 2;
            this.btn_CookConvex.Text = "Cook Convex";
            this.btn_CookConvex.UseVisualStyleBackColor = true;
            this.btn_CookConvex.Click += new System.EventHandler(this.btn_CookConvex_Click);
            // 
            // btn_CookConcave
            // 
            this.btn_CookConcave.Location = new System.Drawing.Point(12, 180);
            this.btn_CookConcave.Name = "btn_CookConcave";
            this.btn_CookConcave.Size = new System.Drawing.Size(185, 34);
            this.btn_CookConcave.TabIndex = 2;
            this.btn_CookConcave.Text = "Cook Concave";
            this.btn_CookConcave.UseVisualStyleBackColor = true;
            this.btn_CookConcave.Click += new System.EventHandler(this.btn_CookConcave_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(209, 225);
            this.Controls.Add(this.btn_CookConcave);
            this.Controls.Add(this.btn_CookConvex);
            this.Controls.Add(this.btn_CookBin);
            this.Controls.Add(this.pB_LoadObj);
            this.Controls.Add(this.btn_LoadObj);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "HappyCooker";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btn_LoadObj;
        private System.Windows.Forms.ProgressBar pB_LoadObj;
        private System.Windows.Forms.Button btn_CookBin;
        private System.Windows.Forms.Button btn_CookConvex;
        private System.Windows.Forms.Button btn_CookConcave;
    }
}

