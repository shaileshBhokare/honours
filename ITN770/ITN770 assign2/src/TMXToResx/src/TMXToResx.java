import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import javax.swing.JOptionPane;

import org.jdom.Attribute;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.JDOMException;
import org.jdom.Namespace;
import org.jdom.input.SAXBuilder;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;


public class TMXToResx {

	private static File TMX, Resx;
	private static String locale;
	private static String usage = "Usage: Three inputs values are required in this order: " +
			"\n\r\t\tcentral database file, locale and basename." +
			"\n\r\tFor example: java TMXToResx storage.tmx ja-JP UIStrings";
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		if(args.length!=3) {
			System.out.println(usage);
		}else {
			TMX = new File(args[0]);		
			locale = args[1];
			Resx = new File(args[2]+"."+locale+".resx");
			
			try {
				/*TMX = new File("C:\\Localization Projects\\Pack1\\Work\\UIStrings.en-AU.resx.tmx");
				Resx = new File("C:\\Localization Projects\\Pack1\\Work\\copy.resx");
				locale = "ja-JP";*/
				FileWriter ResxWriter = new FileWriter(Resx);
				
				//create the XML builder
				SAXBuilder builder = new SAXBuilder();
				
				//create input TMXDoc and output ResxDoc
				//build TMXDoc from exiting TXM file
				Document TMXDoc = builder.build(TMX);
				
				//build ResxDoc from scratch
				Document ResxDoc = new Document(new Element("root"));
				
				//create the basic resx file structure
				Element ResxRoot = ResxDoc.getRootElement();
				Element resheader1 = new Element("resheader")
										.setAttribute("name", "resmimetype")
										.addContent(new Element("value")
														.setText("text/microsoft-resx"));
				Element resheader2 = new Element("resheader")
										.setAttribute("name", "version")
										.addContent(new Element("value")
														.setText("2.0"));
				Element resheader3 = new Element("resheader")
										.setAttribute("name","reader")
										.addContent(new Element("value")
														.setText("System.Resources.ResXResourceReader, System.Windows.Forms, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089"));
				Element resheader4 = new Element("resheader")
										.setAttribute("name","writer")
										.addContent(new Element("value")
											.setText("System.Resources.ResXResourceWriter, System.Windows.Forms, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089"));
				ResxRoot.addContent(resheader1);
				ResxRoot.addContent(resheader2);
				ResxRoot.addContent(resheader3);
				ResxRoot.addContent(resheader4);
				//end of basic structure
				
				//get body element which contains the TU set
				Element root = TMXDoc.getRootElement();
				Element body = root.getChild("body");
				
				//create namespace
				//Namespace xml = Namespace.getNamespace("xml", "");
				
				//get all the children elements and iterate the TU set
				List allTUs = body.getChildren();
				Iterator tuIterator = allTUs.listIterator();
				while(tuIterator.hasNext()){
					Element tu = (Element)tuIterator.next();
					
					//abstract the key
					String key = tu.getAttributeValue("tuid");
					
					//get all the children elemens an iterate them
					List allTuv = tu.getChildren();
					Iterator tuvIterator = allTuv.listIterator();
					while(tuvIterator.hasNext()){
						Element tuv = (Element)tuvIterator.next();
						List attributes = tuv.getAttributes();
						
						/*for(int i=0; i<attributes.size();i++) {
							Attribute a = (Attribute) attributes.get(i);
							System.out.println(a.getValue());
						}*/
						
						Attribute language = (Attribute)attributes.get(0);
						//JOptionPane.showMessageDialog(null, language.getValue());
						if(language.getValue().equals(locale)){
							//JOptionPane.showMessageDialog(null, "up here");
							//abstract the inner text
							String word = tuv.getChildText("seg");
							
							//output the inner text to resx file
							Element data = new Element("data")
										   .setAttribute("name", key)
										   .setAttribute("space", "xml:preserve")
										   .addContent(new Element("value")
										   				.setText(word));
							ResxRoot.addContent(data);
							//System.out.println(data);
						}
					}
				}
				
				//out resx file
				Format format = Format.getPrettyFormat();
				XMLOutputter outputter = new XMLOutputter(format);
				outputter.output(ResxDoc, ResxWriter);
			} catch (JDOMException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
