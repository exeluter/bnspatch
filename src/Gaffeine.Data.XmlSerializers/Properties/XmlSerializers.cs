#if _DYNAMIC_XMLSERIALIZER_COMPILATION
[assembly:System.Security.AllowPartiallyTrustedCallers()]
[assembly:System.Security.SecurityTransparent()]
[assembly:System.Security.SecurityRules(System.Security.SecurityRuleSet.Level1)]
#endif
[assembly:System.Reflection.AssemblyVersionAttribute("1.0.0.0")]
[assembly:System.Xml.Serialization.XmlSerializerVersionAttribute(ParentAssemblyId=@"567131ee-5c2a-48a6-b619-48590a1bc931,", Version=@"4.0.0.0")]
namespace Microsoft.Xml.Serialization.GeneratedAssembly {

    public class XmlSerializationWriter1 : System.Xml.Serialization.XmlSerializationWriter {

        public void Write18_BusinessModel(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"BusinessModel", @"");
                return;
            }
            TopLevelElement();
            Write2_BusinessModel(@"BusinessModel", @"", ((global::Gaffeine.Data.Models.XmlGames.BusinessModel)o), true, false);
        }

        public void Write19_ExternalAuthentication(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"ExternalAuthentication", @"");
                return;
            }
            TopLevelElement();
            Write3_ExternalAuthentication(@"ExternalAuthentication", @"", ((global::Gaffeine.Data.Models.XmlGames.ExternalAuthentication)o), true, false);
        }

        public void Write20_Game(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"Game", @"");
                return;
            }
            TopLevelElement();
            Write9_Game(@"Game", @"", ((global::Gaffeine.Data.Models.XmlGames.Game)o), true, false);
        }

        public void Write21_Games(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"Games", @"");
                return;
            }
            TopLevelElement();
            Write16_Games(@"Games", @"", ((global::Gaffeine.Data.Models.XmlGames.Games)o), true, false);
        }

        public void Write22_L1UpdaterInfo(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"L1UpdaterInfo", @"");
                return;
            }
            TopLevelElement();
            Write7_L1UpdaterInfo(@"L1UpdaterInfo", @"", ((global::Gaffeine.Data.Models.XmlGames.L1UpdaterInfo)o), true, false);
        }

        public void Write23_Launcher(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"Launcher", @"");
                return;
            }
            TopLevelElement();
            Write15_Launcher(@"Launcher", @"", ((global::Gaffeine.Data.Models.XmlGames.Launcher)o), true, false);
        }

        public void Write24_Login(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"Login", @"");
                return;
            }
            TopLevelElement();
            Write10_Login(@"Login", @"", ((global::Gaffeine.Data.Models.XmlGames.Login)o), true, false);
        }

        public void Write25_MenuItem(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"MenuItem", @"");
                return;
            }
            TopLevelElement();
            Write5_MenuItem(@"MenuItem", @"", ((global::Gaffeine.Data.Models.XmlGames.MenuItem)o), true, false);
        }

        public void Write26_NCStreaming(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"NCStreaming", @"");
                return;
            }
            TopLevelElement();
            Write14_NCStreaming(@"NCStreaming", @"", ((global::Gaffeine.Data.Models.XmlGames.NCStreaming)o), true, false);
        }

        public void Write27_NCStreamingSource(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"NCStreamingSource", @"");
                return;
            }
            TopLevelElement();
            Write13_NCStreamingSource(@"NCStreamingSource", @"", ((global::Gaffeine.Data.Models.XmlGames.NCStreamingSource)o), true, false);
        }

        public void Write28_Pattern(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"Pattern", @"");
                return;
            }
            TopLevelElement();
            Write17_Pattern(@"Pattern", @"", ((global::Gaffeine.Data.Models.XmlGames.Pattern)o), true, false);
        }

        public void Write29_Profile(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"Profile", @"");
                return;
            }
            TopLevelElement();
            Write12_Profile(@"Profile", @"", ((global::Gaffeine.Data.Models.XmlGames.Profile)o), true, false);
        }

        public void Write30_ProfileUriInfo(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"ProfileUriInfo", @"");
                return;
            }
            TopLevelElement();
            Write11_ProfileUriInfo(@"ProfileUriInfo", @"", ((global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)o), true, false);
        }

        public void Write31_WebContentFilters(object o) {
            WriteStartDocument();
            if (o == null) {
                WriteNullTagLiteral(@"WebContentFilters", @"");
                return;
            }
            TopLevelElement();
            Write4_WebContentFilters(@"WebContentFilters", @"", ((global::Gaffeine.Data.Models.XmlGames.WebContentFilters)o), true, false);
        }

        void Write4_WebContentFilters(string n, string ns, global::Gaffeine.Data.Models.XmlGames.WebContentFilters o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.WebContentFilters)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"WebContentFilters", @"");
            {
                global::System.Collections.Generic.List<global::System.String> a = (global::System.Collections.Generic.List<global::System.String>)((global::System.Collections.Generic.List<global::System.String>)o.@InternalBrowsingUris);
                if (a != null){
                    WriteStartElement(@"InternalBrowsingUris", @"", null, false);
                    for (int ia = 0; ia < ((System.Collections.ICollection)a).Count; ia++) {
                        WriteNullableStringLiteral(@"UriRegex", @"", ((global::System.String)a[ia]));
                    }
                    WriteEndElement();
                }
            }
            {
                global::System.Collections.Generic.List<global::System.String> a = (global::System.Collections.Generic.List<global::System.String>)((global::System.Collections.Generic.List<global::System.String>)o.@ExcludeInternalBrowsingUris);
                if (a != null){
                    WriteStartElement(@"ExcludeInternalBrowsingUris", @"", null, false);
                    for (int ia = 0; ia < ((System.Collections.ICollection)a).Count; ia++) {
                        WriteNullableStringLiteral(@"UriRegex", @"", ((global::System.String)a[ia]));
                    }
                    WriteEndElement();
                }
            }
            WriteEndElement(o);
        }

        void Write11_ProfileUriInfo(string n, string ns, global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"ProfileUriInfo", @"");
            WriteAttribute(@"Uri", @"", ((global::System.String)o.@Uri));
            WriteAttribute(@"IsExternal", @"", System.Xml.XmlConvert.ToString((global::System.Boolean)((global::System.Boolean)o.@IsExternal)));
            WriteAttribute(@"WindowTitleResourceKey", @"", ((global::System.String)o.@WindowTitleResourceKey));
            WriteEndElement(o);
        }

        void Write12_Profile(string n, string ns, global::Gaffeine.Data.Models.XmlGames.Profile o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.Profile)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"Profile", @"");
            Write11_ProfileUriInfo(@"Charge", @"", ((global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)o.@Charge), false, false);
            Write11_ProfileUriInfo(@"Present", @"", ((global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)o.@Present), false, false);
            Write11_ProfileUriInfo(@"MyPage", @"", ((global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)o.@MyPage), false, false);
            Write11_ProfileUriInfo(@"Content", @"", ((global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)o.@Content), false, false);
            Write11_ProfileUriInfo(@"Coupon", @"", ((global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)o.@Coupon), false, false);
            Write11_ProfileUriInfo(@"Security", @"", ((global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)o.@Security), false, false);
            Write11_ProfileUriInfo(@"Center", @"", ((global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)o.@Center), false, false);
            WriteEndElement(o);
        }

        void Write17_Pattern(string n, string ns, global::Gaffeine.Data.Models.XmlGames.Pattern o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.Pattern)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"Pattern", @"");
            if ((object)(o.@Value) != null){
                WriteValue(((global::System.String)o.@Value));
            }
            WriteEndElement(o);
        }

        void Write13_NCStreamingSource(string n, string ns, global::Gaffeine.Data.Models.XmlGames.NCStreamingSource o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.NCStreamingSource)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"NCStreamingSource", @"");
            WriteAttribute(@"Title", @"", ((global::System.String)o.@Title));
            WriteAttribute(@"FacebookTag", @"", ((global::System.String)o.@FacebookTag));
            WriteAttribute(@"GameName", @"", ((global::System.String)o.@GameName));
            WriteEndElement(o);
        }

        void Write14_NCStreaming(string n, string ns, global::Gaffeine.Data.Models.XmlGames.NCStreaming o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.NCStreaming)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"NCStreaming", @"");
            WriteAttribute(@"ExePath", @"", ((global::System.String)o.@ExePath));
            {
                global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.NCStreamingSource> a = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.NCStreamingSource>)((global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.NCStreamingSource>)o.@Sources);
                if (a != null){
                    WriteStartElement(@"Sources", @"", null, false);
                    for (int ia = 0; ia < ((System.Collections.ICollection)a).Count; ia++) {
                        Write13_NCStreamingSource(@"Source", @"", ((global::Gaffeine.Data.Models.XmlGames.NCStreamingSource)a[ia]), true, false);
                    }
                    WriteEndElement();
                }
            }
            WriteEndElement(o);
        }

        void Write5_MenuItem(string n, string ns, global::Gaffeine.Data.Models.XmlGames.MenuItem o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.MenuItem)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"MenuItem", @"");
            WriteAttribute(@"Id", @"", ((global::System.String)o.@Id));
            WriteAttribute(@"Title", @"", ((global::System.String)o.@Title));
            WriteAttribute(@"Image", @"", ((global::System.String)o.@Image));
            WriteAttribute(@"Uri", @"", ((global::System.String)o.@Uri));
            WriteAttribute(@"External_Browser", @"", System.Xml.XmlConvert.ToString((global::System.Boolean)((global::System.Boolean)o.@ExternalBrowser)));
            WriteEndElement(o);
        }

        void Write10_Login(string n, string ns, global::Gaffeine.Data.Models.XmlGames.Login o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.Login)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"Login", @"");
            WriteAttribute(@"CreateAccountUri", @"", ((global::System.String)o.@CreateAccountUri));
            WriteAttribute(@"CantLoginUri", @"", ((global::System.String)o.@CantLoginUri));
            WriteEndElement(o);
        }

        void Write15_Launcher(string n, string ns, global::Gaffeine.Data.Models.XmlGames.Launcher o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.Launcher)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"Launcher", @"");
            WriteAttribute(@"Title", @"", ((global::System.String)o.@Title));
            WriteAttribute(@"ResFolder", @"", ((global::System.String)o.@ResourceId));
            WriteAttribute(@"DownloadFolder", @"", ((global::System.String)o.@DownloadFolder));
            WriteAttribute(@"GameID", @"", ((global::System.String)o.@GameId));
            WriteAttribute(@"UpdateSrvAddr", @"", ((global::System.String)o.@UpdateServerAddress));
            WriteAttribute(@"LoginWebUri", @"", ((global::System.String)o.@LoginWebUri));
            WriteAttribute(@"LoginWebAppId", @"", ((global::System.String)o.@LoginWebAppID));
            WriteAttribute(@"AccountWebUri", @"", ((global::System.String)o.@AccountWebUri));
            WriteAttribute(@"NCComixUri", @"", ((global::System.String)o.@NCComixUri));
            WriteAttribute(@"BuffToonUri", @"", ((global::System.String)o.@BuffToonUri));
            WriteAttribute(@"HelpUri", @"", ((global::System.String)o.@HelpUri));
            WriteAttribute(@"PlayncUri", @"", ((global::System.String)o.@PlayncUri));
            WriteAttribute(@"OpenBetaEventUri", @"", ((global::System.String)o.@OpenBetaEventUri));
            WriteAttribute(@"TeenServiceGamePlayTimeUri", @"", ((global::System.String)o.@TeenServiceGamePlayTimeUri));
            WriteAttribute(@"DownloadPageUri", @"", ((global::System.String)o.@DownloadPageUri));
            WriteAttribute(@"OverrideCss", @"", ((global::System.String)o.@OverrideCss));
            Write10_Login(@"Login", @"", ((global::Gaffeine.Data.Models.XmlGames.Login)o.@Login), false, false);
            Write3_ExternalAuthentication(@"ExternalAuthentication", @"", ((global::Gaffeine.Data.Models.XmlGames.ExternalAuthentication)o.@ExternalAuthentication), false, false);
            Write12_Profile(@"Profile", @"", ((global::Gaffeine.Data.Models.XmlGames.Profile)o.@Profile), false, false);
            Write14_NCStreaming(@"NCStreaming", @"", ((global::Gaffeine.Data.Models.XmlGames.NCStreaming)o.@NCStreaming), false, false);
            {
                global::System.Collections.Generic.List<global::System.String> a = (global::System.Collections.Generic.List<global::System.String>)((global::System.Collections.Generic.List<global::System.String>)o.@SsoDomains);
                if (a != null){
                    WriteStartElement(@"SsoDomains", @"", null, false);
                    for (int ia = 0; ia < ((System.Collections.ICollection)a).Count; ia++) {
                        WriteNullableStringLiteral(@"DomainRegex", @"", ((global::System.String)a[ia]));
                    }
                    WriteEndElement();
                }
            }
            WriteEndElement(o);
        }

        void Write3_ExternalAuthentication(string n, string ns, global::Gaffeine.Data.Models.XmlGames.ExternalAuthentication o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.ExternalAuthentication)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"ExternalAuthentication", @"");
            WriteAttribute(@"Facebook", @"", System.Xml.XmlConvert.ToString((global::System.Boolean)((global::System.Boolean)o.@Facebook)));
            WriteAttribute(@"Google", @"", System.Xml.XmlConvert.ToString((global::System.Boolean)((global::System.Boolean)o.@Google)));
            WriteEndElement(o);
        }

        void Write7_L1UpdaterInfo(string n, string ns, global::Gaffeine.Data.Models.XmlGames.L1UpdaterInfo o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.L1UpdaterInfo)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"L1UpdaterInfo", @"");
            WriteAttribute(@"ServerType", @"", Write6_L1UpdateServerNetwork(((global::Gaffeine.Data.Enums.L1UpdateServerNetwork)o.@ServerType)));
            WriteEndElement(o);
        }

        string Write6_L1UpdateServerNetwork(global::Gaffeine.Data.Enums.L1UpdateServerNetwork v) {
            string s = null;
            switch (v) {
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@DEV_ALPHA: s = @"DEV_ALPHA"; break;
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@DEV_BETA: s = @"DEV_BETA"; break;
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@SANDBOX_BETA: s = @"SANDBOX_BETA"; break;
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@RC: s = @"RC"; break;
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@RC_TEST: s = @"RC_TEST"; break;
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@CLONE: s = @"CLONE"; break;
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@TEST: s = @"TEST"; break;
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@LIVE_KOREA: s = @"LIVE_KOREA"; break;
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@LIVE_JAPAN: s = @"LIVE_JAPAN"; break;
                case global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@LIVE_TAIWAN: s = @"LIVE_TAIWAN"; break;
                default: throw CreateInvalidEnumValueException(((System.Int64)v).ToString(System.Globalization.CultureInfo.InvariantCulture), @"Gaffeine.Data.Enums.L1UpdateServerNetwork");
            }
            return s;
        }

        void Write16_Games(string n, string ns, global::Gaffeine.Data.Models.XmlGames.Games o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.Games)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"Games", @"");
            Write15_Launcher(@"Launcher", @"", ((global::Gaffeine.Data.Models.XmlGames.Launcher)o.@Launcher), false, false);
            {
                global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game> a = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game>)o.@Game;
                if (a != null) {
                    for (int ia = 0; ia < ((System.Collections.ICollection)a).Count; ia++) {
                        Write9_Game(@"Game", @"", ((global::Gaffeine.Data.Models.XmlGames.Game)a[ia]), false, false);
                    }
                }
            }
            WriteEndElement(o);
        }

        void Write9_Game(string n, string ns, global::Gaffeine.Data.Models.XmlGames.Game o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.Game)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"Game", @"");
            WriteAttribute(@"IsRepresentative", @"", System.Xml.XmlConvert.ToString((global::System.Boolean)((global::System.Boolean)o.@IsRepresentative)));
            WriteAttribute(@"Title", @"", ((global::System.String)o.@Title));
            WriteAttribute(@"GameServerName", @"", ((global::System.String)o.@GameServerName));
            WriteAttribute(@"ResFolder", @"", ((global::System.String)o.@ResourceId));
            WriteAttribute(@"DownloadFolder", @"", ((global::System.String)o.@DownloadFolder));
            WriteAttribute(@"GameID", @"", ((global::System.String)o.@GameId));
            WriteAttribute(@"UpdateSrvAddr", @"", ((global::System.String)o.@UpdateServerAddress));
            WriteAttribute(@"AppId", @"", ((global::System.String)o.@AppId));
            WriteAttribute(@"AppGroupId", @"", ((global::System.String)o.@AppGroupId));
            WriteAttribute(@"AllowMultiClient", @"", ((global::System.String)o.@AllowMultiClient));
            WriteAttribute(@"AllowOverlayAgent", @"", ((global::System.String)o.@AllowOverlayAgent));
            WriteAttribute(@"OverlayGameSignature", @"", ((global::System.String)o.@OverlayGameSignature));
            WriteAttribute(@"CrossPromotionAppGroupCode", @"", ((global::System.String)o.@CrossPromotionAppGroupCode));
            WriteAttribute(@"CrossPromotionMissionKey", @"", ((global::System.String)o.@CrossPromotionMissionKey));
            WriteAttribute(@"GameRatingAge", @"", ((global::System.String)o.@GameRatingAge));
            WriteAttribute(@"NetworkScope", @"", ((global::System.String)o.@NetworkScope));
            WriteAttribute(@"CreateAccountUri", @"", ((global::System.String)o.@CreateAccountUri));
            Write4_WebContentFilters(@"WebContentFilters", @"", ((global::Gaffeine.Data.Models.XmlGames.WebContentFilters)o.@WebContentFilters), false, false);
            {
                global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.MenuItem> a = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.MenuItem>)((global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.MenuItem>)o.@Menu);
                if (a != null){
                    WriteStartElement(@"Menu", @"", null, false);
                    for (int ia = 0; ia < ((System.Collections.ICollection)a).Count; ia++) {
                        Write5_MenuItem(@"MenuItem", @"", ((global::Gaffeine.Data.Models.XmlGames.MenuItem)a[ia]), true, false);
                    }
                    WriteEndElement();
                }
            }
            {
                global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game> a = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game>)((global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game>)o.@TestGame);
                if (a != null){
                    WriteStartElement(@"TestServerGames", @"", null, false);
                    for (int ia = 0; ia < ((System.Collections.ICollection)a).Count; ia++) {
                        Write9_Game(@"TestGame", @"", ((global::Gaffeine.Data.Models.XmlGames.Game)a[ia]), true, false);
                    }
                    WriteEndElement();
                }
            }
            Write7_L1UpdaterInfo(@"L1UpdaterInfo", @"", ((global::Gaffeine.Data.Models.XmlGames.L1UpdaterInfo)o.@L1UpdaterInfo), false, false);
            {
                global::System.Collections.Generic.List<global::System.String> a = (global::System.Collections.Generic.List<global::System.String>)((global::System.Collections.Generic.List<global::System.String>)o.@AlternativeRegistrySubKeys);
                if (a != null){
                    WriteStartElement(@"AlternativeRegistrySubKeys", @"", null, false);
                    for (int ia = 0; ia < ((System.Collections.ICollection)a).Count; ia++) {
                        WriteNullableStringLiteral(@"SubKey", @"", ((global::System.String)a[ia]));
                    }
                    WriteEndElement();
                }
            }
            Write2_BusinessModel(@"BusinessModel", @"", ((global::Gaffeine.Data.Models.XmlGames.BusinessModel)o.@BusinessModel), false, false);
            {
                global::System.Collections.Generic.List<global::Gaffeine.Data.Enums.PublishContinent> a = (global::System.Collections.Generic.List<global::Gaffeine.Data.Enums.PublishContinent>)((global::System.Collections.Generic.List<global::Gaffeine.Data.Enums.PublishContinent>)o.@PublishContinents);
                if (a != null){
                    WriteStartElement(@"PublishContinents", @"", null, false);
                    for (int ia = 0; ia < ((System.Collections.ICollection)a).Count; ia++) {
                        WriteElementString(@"Continent", @"", Write8_PublishContinent(((global::Gaffeine.Data.Enums.PublishContinent)a[ia])));
                    }
                    WriteEndElement();
                }
            }
            WriteEndElement(o);
        }

        string Write8_PublishContinent(global::Gaffeine.Data.Enums.PublishContinent v) {
            string s = null;
            switch (v) {
                case global::Gaffeine.Data.Enums.PublishContinent.@NONE: s = @"NONE"; break;
                case global::Gaffeine.Data.Enums.PublishContinent.@NA: s = @"NA"; break;
                case global::Gaffeine.Data.Enums.PublishContinent.@SA: s = @"SA"; break;
                case global::Gaffeine.Data.Enums.PublishContinent.@EU: s = @"EU"; break;
                case global::Gaffeine.Data.Enums.PublishContinent.@OC: s = @"OC"; break;
                default: throw CreateInvalidEnumValueException(((System.Int64)v).ToString(System.Globalization.CultureInfo.InvariantCulture), @"Gaffeine.Data.Enums.PublishContinent");
            }
            return s;
        }

        void Write2_BusinessModel(string n, string ns, global::Gaffeine.Data.Models.XmlGames.BusinessModel o, bool isNullable, bool needType) {
            if ((object)o == null) {
                if (isNullable) WriteNullTagLiteral(n, ns);
                return;
            }
            if (!needType) {
                System.Type t = o.GetType();
                if (t == typeof(global::Gaffeine.Data.Models.XmlGames.BusinessModel)) {
                }
                else {
                    throw CreateUnknownTypeException(o);
                }
            }
            WriteStartElement(n, ns, o, false, null);
            if (needType) WriteXsiType(@"BusinessModel", @"");
            WriteElementString(@"BusinessModelKind", @"", ((global::System.String)o.@BusinessModelKind));
            WriteElementString(@"AppGroupCode", @"", ((global::System.String)o.@AppGroupCode));
            WriteElementString(@"PurchaseUri", @"", ((global::System.String)o.@PurchaseUri));
            WriteEndElement(o);
        }

        protected override void InitCallbacks() {
        }
    }

    public class XmlSerializationReader1 : System.Xml.Serialization.XmlSerializationReader {

        public object Read18_BusinessModel() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id1_BusinessModel && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read2_BusinessModel(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":BusinessModel");
            }
            return (object)o;
        }

        public object Read19_ExternalAuthentication() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id3_ExternalAuthentication && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read3_ExternalAuthentication(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":ExternalAuthentication");
            }
            return (object)o;
        }

        public object Read20_Game() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id4_Game && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read9_Game(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":Game");
            }
            return (object)o;
        }

        public object Read21_Games() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id5_Games && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read16_Games(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":Games");
            }
            return (object)o;
        }

        public object Read22_L1UpdaterInfo() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id6_L1UpdaterInfo && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read7_L1UpdaterInfo(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":L1UpdaterInfo");
            }
            return (object)o;
        }

        public object Read23_Launcher() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id7_Launcher && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read15_Launcher(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":Launcher");
            }
            return (object)o;
        }

        public object Read24_Login() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id8_Login && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read10_Login(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":Login");
            }
            return (object)o;
        }

        public object Read25_MenuItem() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id9_MenuItem && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read5_MenuItem(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":MenuItem");
            }
            return (object)o;
        }

        public object Read26_NCStreaming() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id10_NCStreaming && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read14_NCStreaming(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":NCStreaming");
            }
            return (object)o;
        }

        public object Read27_NCStreamingSource() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id11_NCStreamingSource && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read13_NCStreamingSource(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":NCStreamingSource");
            }
            return (object)o;
        }

        public object Read28_Pattern() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id12_Pattern && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read17_Pattern(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":Pattern");
            }
            return (object)o;
        }

        public object Read29_Profile() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id13_Profile && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read12_Profile(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":Profile");
            }
            return (object)o;
        }

        public object Read30_ProfileUriInfo() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id14_ProfileUriInfo && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read11_ProfileUriInfo(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":ProfileUriInfo");
            }
            return (object)o;
        }

        public object Read31_WebContentFilters() {
            object o = null;
            Reader.MoveToContent();
            if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                if (((object) Reader.LocalName == (object)id15_WebContentFilters && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o = Read4_WebContentFilters(true, true);
                }
                else {
                    throw CreateUnknownNodeException();
                }
            }
            else {
                UnknownNode(null, @":WebContentFilters");
            }
            return (object)o;
        }

        global::Gaffeine.Data.Models.XmlGames.WebContentFilters Read4_WebContentFilters(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id15_WebContentFilters && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.WebContentFilters o;
            o = new global::Gaffeine.Data.Models.XmlGames.WebContentFilters();
            if ((object)(o.@InternalBrowsingUris) == null) o.@InternalBrowsingUris = new global::System.Collections.Generic.List<global::System.String>();
            global::System.Collections.Generic.List<global::System.String> a_0 = (global::System.Collections.Generic.List<global::System.String>)o.@InternalBrowsingUris;
            if ((object)(o.@ExcludeInternalBrowsingUris) == null) o.@ExcludeInternalBrowsingUris = new global::System.Collections.Generic.List<global::System.String>();
            global::System.Collections.Generic.List<global::System.String> a_1 = (global::System.Collections.Generic.List<global::System.String>)o.@ExcludeInternalBrowsingUris;
            bool[] paramsRead = new bool[2];
            while (Reader.MoveToNextAttribute()) {
                if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o);
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations0 = 0;
            int readerCount0 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    if (((object) Reader.LocalName == (object)id16_InternalBrowsingUris && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        if (!ReadNull()) {
                            if ((object)(o.@InternalBrowsingUris) == null) o.@InternalBrowsingUris = new global::System.Collections.Generic.List<global::System.String>();
                            global::System.Collections.Generic.List<global::System.String> a_0_0 = (global::System.Collections.Generic.List<global::System.String>)o.@InternalBrowsingUris;
                            if ((Reader.IsEmptyElement)) {
                                Reader.Skip();
                            }
                            else {
                                Reader.ReadStartElement();
                                Reader.MoveToContent();
                                int whileIterations1 = 0;
                                int readerCount1 = ReaderCount;
                                while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                                    if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                                        if (((object) Reader.LocalName == (object)id17_UriRegex && (object) Reader.NamespaceURI == (object)id2_Item)) {
                                            if (ReadNull()) {
                                                a_0_0.Add(null);
                                            }
                                            else {
                                                a_0_0.Add(Reader.ReadElementString());
                                            }
                                        }
                                        else {
                                            UnknownNode(null, @":UriRegex");
                                        }
                                    }
                                    else {
                                        UnknownNode(null, @":UriRegex");
                                    }
                                    Reader.MoveToContent();
                                    CheckReaderCount(ref whileIterations1, ref readerCount1);
                                }
                            ReadEndElement();
                            }
                        }
                    }
                    else if (((object) Reader.LocalName == (object)id18_ExcludeInternalBrowsingUris && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        if (!ReadNull()) {
                            if ((object)(o.@ExcludeInternalBrowsingUris) == null) o.@ExcludeInternalBrowsingUris = new global::System.Collections.Generic.List<global::System.String>();
                            global::System.Collections.Generic.List<global::System.String> a_1_0 = (global::System.Collections.Generic.List<global::System.String>)o.@ExcludeInternalBrowsingUris;
                            if ((Reader.IsEmptyElement)) {
                                Reader.Skip();
                            }
                            else {
                                Reader.ReadStartElement();
                                Reader.MoveToContent();
                                int whileIterations2 = 0;
                                int readerCount2 = ReaderCount;
                                while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                                    if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                                        if (((object) Reader.LocalName == (object)id17_UriRegex && (object) Reader.NamespaceURI == (object)id2_Item)) {
                                            if (ReadNull()) {
                                                a_1_0.Add(null);
                                            }
                                            else {
                                                a_1_0.Add(Reader.ReadElementString());
                                            }
                                        }
                                        else {
                                            UnknownNode(null, @":UriRegex");
                                        }
                                    }
                                    else {
                                        UnknownNode(null, @":UriRegex");
                                    }
                                    Reader.MoveToContent();
                                    CheckReaderCount(ref whileIterations2, ref readerCount2);
                                }
                            ReadEndElement();
                            }
                        }
                    }
                    else {
                        UnknownNode((object)o, @":InternalBrowsingUris, :ExcludeInternalBrowsingUris");
                    }
                }
                else {
                    UnknownNode((object)o, @":InternalBrowsingUris, :ExcludeInternalBrowsingUris");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations0, ref readerCount0);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo Read11_ProfileUriInfo(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id14_ProfileUriInfo && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo o;
            o = new global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo();
            bool[] paramsRead = new bool[3];
            while (Reader.MoveToNextAttribute()) {
                if (!paramsRead[0] && ((object) Reader.LocalName == (object)id19_Uri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Uri = Reader.Value;
                    paramsRead[0] = true;
                }
                else if (!paramsRead[1] && ((object) Reader.LocalName == (object)id20_IsExternal && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@IsExternal = System.Xml.XmlConvert.ToBoolean(Reader.Value);
                    paramsRead[1] = true;
                }
                else if (!paramsRead[2] && ((object) Reader.LocalName == (object)id21_WindowTitleResourceKey && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@WindowTitleResourceKey = Reader.Value;
                    paramsRead[2] = true;
                }
                else if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o, @":Uri, :IsExternal, :WindowTitleResourceKey");
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations3 = 0;
            int readerCount3 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    UnknownNode((object)o, @"");
                }
                else {
                    UnknownNode((object)o, @"");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations3, ref readerCount3);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.Profile Read12_Profile(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id13_Profile && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.Profile o;
            o = new global::Gaffeine.Data.Models.XmlGames.Profile();
            bool[] paramsRead = new bool[7];
            while (Reader.MoveToNextAttribute()) {
                if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o);
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations4 = 0;
            int readerCount4 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    if (!paramsRead[0] && ((object) Reader.LocalName == (object)id22_Charge && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@Charge = Read11_ProfileUriInfo(false, true);
                        paramsRead[0] = true;
                    }
                    else if (!paramsRead[1] && ((object) Reader.LocalName == (object)id23_Present && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@Present = Read11_ProfileUriInfo(false, true);
                        paramsRead[1] = true;
                    }
                    else if (!paramsRead[2] && ((object) Reader.LocalName == (object)id24_MyPage && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@MyPage = Read11_ProfileUriInfo(false, true);
                        paramsRead[2] = true;
                    }
                    else if (!paramsRead[3] && ((object) Reader.LocalName == (object)id25_Content && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@Content = Read11_ProfileUriInfo(false, true);
                        paramsRead[3] = true;
                    }
                    else if (!paramsRead[4] && ((object) Reader.LocalName == (object)id26_Coupon && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@Coupon = Read11_ProfileUriInfo(false, true);
                        paramsRead[4] = true;
                    }
                    else if (!paramsRead[5] && ((object) Reader.LocalName == (object)id27_Security && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@Security = Read11_ProfileUriInfo(false, true);
                        paramsRead[5] = true;
                    }
                    else if (!paramsRead[6] && ((object) Reader.LocalName == (object)id28_Center && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@Center = Read11_ProfileUriInfo(false, true);
                        paramsRead[6] = true;
                    }
                    else {
                        UnknownNode((object)o, @":Charge, :Present, :MyPage, :Content, :Coupon, :Security, :Center");
                    }
                }
                else {
                    UnknownNode((object)o, @":Charge, :Present, :MyPage, :Content, :Coupon, :Security, :Center");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations4, ref readerCount4);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.Pattern Read17_Pattern(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id12_Pattern && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.Pattern o;
            o = new global::Gaffeine.Data.Models.XmlGames.Pattern();
            bool[] paramsRead = new bool[1];
            while (Reader.MoveToNextAttribute()) {
                if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o);
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations5 = 0;
            int readerCount5 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                string tmp = null;
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    UnknownNode((object)o, @"");
                }
                else if (Reader.NodeType == System.Xml.XmlNodeType.Text || 
                Reader.NodeType == System.Xml.XmlNodeType.CDATA || 
                Reader.NodeType == System.Xml.XmlNodeType.Whitespace || 
                Reader.NodeType == System.Xml.XmlNodeType.SignificantWhitespace) {
                    tmp = ReadString(tmp, false);
                    o.@Value = tmp;
                }
                else {
                    UnknownNode((object)o, @"");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations5, ref readerCount5);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.NCStreamingSource Read13_NCStreamingSource(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id11_NCStreamingSource && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.NCStreamingSource o;
            o = new global::Gaffeine.Data.Models.XmlGames.NCStreamingSource();
            bool[] paramsRead = new bool[3];
            while (Reader.MoveToNextAttribute()) {
                if (!paramsRead[0] && ((object) Reader.LocalName == (object)id29_Title && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Title = Reader.Value;
                    paramsRead[0] = true;
                }
                else if (!paramsRead[1] && ((object) Reader.LocalName == (object)id30_FacebookTag && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@FacebookTag = Reader.Value;
                    paramsRead[1] = true;
                }
                else if (!paramsRead[2] && ((object) Reader.LocalName == (object)id31_GameName && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@GameName = Reader.Value;
                    paramsRead[2] = true;
                }
                else if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o, @":Title, :FacebookTag, :GameName");
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations6 = 0;
            int readerCount6 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    UnknownNode((object)o, @"");
                }
                else {
                    UnknownNode((object)o, @"");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations6, ref readerCount6);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.NCStreaming Read14_NCStreaming(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id10_NCStreaming && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.NCStreaming o;
            o = new global::Gaffeine.Data.Models.XmlGames.NCStreaming();
            if ((object)(o.@Sources) == null) o.@Sources = new global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.NCStreamingSource>();
            global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.NCStreamingSource> a_1 = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.NCStreamingSource>)o.@Sources;
            bool[] paramsRead = new bool[2];
            while (Reader.MoveToNextAttribute()) {
                if (!paramsRead[0] && ((object) Reader.LocalName == (object)id32_ExePath && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@ExePath = Reader.Value;
                    paramsRead[0] = true;
                }
                else if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o, @":ExePath");
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations7 = 0;
            int readerCount7 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    if (((object) Reader.LocalName == (object)id33_Sources && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        if (!ReadNull()) {
                            if ((object)(o.@Sources) == null) o.@Sources = new global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.NCStreamingSource>();
                            global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.NCStreamingSource> a_1_0 = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.NCStreamingSource>)o.@Sources;
                            if ((Reader.IsEmptyElement)) {
                                Reader.Skip();
                            }
                            else {
                                Reader.ReadStartElement();
                                Reader.MoveToContent();
                                int whileIterations8 = 0;
                                int readerCount8 = ReaderCount;
                                while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                                    if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                                        if (((object) Reader.LocalName == (object)id34_Source && (object) Reader.NamespaceURI == (object)id2_Item)) {
                                            if ((object)(a_1_0) == null) Reader.Skip(); else a_1_0.Add(Read13_NCStreamingSource(true, true));
                                        }
                                        else {
                                            UnknownNode(null, @":Source");
                                        }
                                    }
                                    else {
                                        UnknownNode(null, @":Source");
                                    }
                                    Reader.MoveToContent();
                                    CheckReaderCount(ref whileIterations8, ref readerCount8);
                                }
                            ReadEndElement();
                            }
                        }
                    }
                    else {
                        UnknownNode((object)o, @":Sources");
                    }
                }
                else {
                    UnknownNode((object)o, @":Sources");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations7, ref readerCount7);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.MenuItem Read5_MenuItem(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id9_MenuItem && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.MenuItem o;
            o = new global::Gaffeine.Data.Models.XmlGames.MenuItem();
            bool[] paramsRead = new bool[5];
            while (Reader.MoveToNextAttribute()) {
                if (!paramsRead[0] && ((object) Reader.LocalName == (object)id35_Id && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Id = Reader.Value;
                    paramsRead[0] = true;
                }
                else if (!paramsRead[1] && ((object) Reader.LocalName == (object)id29_Title && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Title = Reader.Value;
                    paramsRead[1] = true;
                }
                else if (!paramsRead[2] && ((object) Reader.LocalName == (object)id36_Image && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Image = Reader.Value;
                    paramsRead[2] = true;
                }
                else if (!paramsRead[3] && ((object) Reader.LocalName == (object)id19_Uri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Uri = Reader.Value;
                    paramsRead[3] = true;
                }
                else if (!paramsRead[4] && ((object) Reader.LocalName == (object)id37_External_Browser && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@ExternalBrowser = System.Xml.XmlConvert.ToBoolean(Reader.Value);
                    paramsRead[4] = true;
                }
                else if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o, @":Id, :Title, :Image, :Uri, :External_Browser");
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations9 = 0;
            int readerCount9 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    UnknownNode((object)o, @"");
                }
                else {
                    UnknownNode((object)o, @"");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations9, ref readerCount9);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.Login Read10_Login(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id8_Login && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.Login o;
            o = new global::Gaffeine.Data.Models.XmlGames.Login();
            bool[] paramsRead = new bool[2];
            while (Reader.MoveToNextAttribute()) {
                if (!paramsRead[0] && ((object) Reader.LocalName == (object)id38_CreateAccountUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@CreateAccountUri = Reader.Value;
                    paramsRead[0] = true;
                }
                else if (!paramsRead[1] && ((object) Reader.LocalName == (object)id39_CantLoginUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@CantLoginUri = Reader.Value;
                    paramsRead[1] = true;
                }
                else if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o, @":CreateAccountUri, :CantLoginUri");
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations10 = 0;
            int readerCount10 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    UnknownNode((object)o, @"");
                }
                else {
                    UnknownNode((object)o, @"");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations10, ref readerCount10);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.Launcher Read15_Launcher(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id7_Launcher && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.Launcher o;
            o = new global::Gaffeine.Data.Models.XmlGames.Launcher();
            if ((object)(o.@SsoDomains) == null) o.@SsoDomains = new global::System.Collections.Generic.List<global::System.String>();
            global::System.Collections.Generic.List<global::System.String> a_20 = (global::System.Collections.Generic.List<global::System.String>)o.@SsoDomains;
            bool[] paramsRead = new bool[21];
            while (Reader.MoveToNextAttribute()) {
                if (!paramsRead[0] && ((object) Reader.LocalName == (object)id29_Title && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Title = Reader.Value;
                    paramsRead[0] = true;
                }
                else if (!paramsRead[1] && ((object) Reader.LocalName == (object)id40_ResFolder && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@ResourceId = Reader.Value;
                    paramsRead[1] = true;
                }
                else if (!paramsRead[2] && ((object) Reader.LocalName == (object)id41_DownloadFolder && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@DownloadFolder = Reader.Value;
                    paramsRead[2] = true;
                }
                else if (!paramsRead[3] && ((object) Reader.LocalName == (object)id42_GameID && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@GameId = Reader.Value;
                    paramsRead[3] = true;
                }
                else if (!paramsRead[4] && ((object) Reader.LocalName == (object)id43_UpdateSrvAddr && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@UpdateServerAddress = Reader.Value;
                    paramsRead[4] = true;
                }
                else if (!paramsRead[5] && ((object) Reader.LocalName == (object)id44_LoginWebUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@LoginWebUri = Reader.Value;
                    paramsRead[5] = true;
                }
                else if (!paramsRead[6] && ((object) Reader.LocalName == (object)id45_LoginWebAppId && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@LoginWebAppID = Reader.Value;
                    paramsRead[6] = true;
                }
                else if (!paramsRead[7] && ((object) Reader.LocalName == (object)id46_AccountWebUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@AccountWebUri = Reader.Value;
                    paramsRead[7] = true;
                }
                else if (!paramsRead[8] && ((object) Reader.LocalName == (object)id47_NCComixUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@NCComixUri = Reader.Value;
                    paramsRead[8] = true;
                }
                else if (!paramsRead[9] && ((object) Reader.LocalName == (object)id48_BuffToonUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@BuffToonUri = Reader.Value;
                    paramsRead[9] = true;
                }
                else if (!paramsRead[10] && ((object) Reader.LocalName == (object)id49_HelpUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@HelpUri = Reader.Value;
                    paramsRead[10] = true;
                }
                else if (!paramsRead[11] && ((object) Reader.LocalName == (object)id50_PlayncUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@PlayncUri = Reader.Value;
                    paramsRead[11] = true;
                }
                else if (!paramsRead[12] && ((object) Reader.LocalName == (object)id51_OpenBetaEventUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@OpenBetaEventUri = Reader.Value;
                    paramsRead[12] = true;
                }
                else if (!paramsRead[17] && ((object) Reader.LocalName == (object)id52_TeenServiceGamePlayTimeUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@TeenServiceGamePlayTimeUri = Reader.Value;
                    paramsRead[17] = true;
                }
                else if (!paramsRead[18] && ((object) Reader.LocalName == (object)id53_DownloadPageUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@DownloadPageUri = Reader.Value;
                    paramsRead[18] = true;
                }
                else if (!paramsRead[19] && ((object) Reader.LocalName == (object)id54_OverrideCss && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@OverrideCss = Reader.Value;
                    paramsRead[19] = true;
                }
                else if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o, @":Title, :ResFolder, :DownloadFolder, :GameID, :UpdateSrvAddr, :LoginWebUri, :LoginWebAppId, :AccountWebUri, :NCComixUri, :BuffToonUri, :HelpUri, :PlayncUri, :OpenBetaEventUri, :TeenServiceGamePlayTimeUri, :DownloadPageUri, :OverrideCss");
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations11 = 0;
            int readerCount11 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    if (!paramsRead[13] && ((object) Reader.LocalName == (object)id8_Login && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@Login = Read10_Login(false, true);
                        paramsRead[13] = true;
                    }
                    else if (!paramsRead[14] && ((object) Reader.LocalName == (object)id3_ExternalAuthentication && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@ExternalAuthentication = Read3_ExternalAuthentication(false, true);
                        paramsRead[14] = true;
                    }
                    else if (!paramsRead[15] && ((object) Reader.LocalName == (object)id13_Profile && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@Profile = Read12_Profile(false, true);
                        paramsRead[15] = true;
                    }
                    else if (!paramsRead[16] && ((object) Reader.LocalName == (object)id10_NCStreaming && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@NCStreaming = Read14_NCStreaming(false, true);
                        paramsRead[16] = true;
                    }
                    else if (((object) Reader.LocalName == (object)id55_SsoDomains && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        if (!ReadNull()) {
                            if ((object)(o.@SsoDomains) == null) o.@SsoDomains = new global::System.Collections.Generic.List<global::System.String>();
                            global::System.Collections.Generic.List<global::System.String> a_20_0 = (global::System.Collections.Generic.List<global::System.String>)o.@SsoDomains;
                            if ((Reader.IsEmptyElement)) {
                                Reader.Skip();
                            }
                            else {
                                Reader.ReadStartElement();
                                Reader.MoveToContent();
                                int whileIterations12 = 0;
                                int readerCount12 = ReaderCount;
                                while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                                    if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                                        if (((object) Reader.LocalName == (object)id56_DomainRegex && (object) Reader.NamespaceURI == (object)id2_Item)) {
                                            if (ReadNull()) {
                                                a_20_0.Add(null);
                                            }
                                            else {
                                                a_20_0.Add(Reader.ReadElementString());
                                            }
                                        }
                                        else {
                                            UnknownNode(null, @":DomainRegex");
                                        }
                                    }
                                    else {
                                        UnknownNode(null, @":DomainRegex");
                                    }
                                    Reader.MoveToContent();
                                    CheckReaderCount(ref whileIterations12, ref readerCount12);
                                }
                            ReadEndElement();
                            }
                        }
                    }
                    else {
                        UnknownNode((object)o, @":Login, :ExternalAuthentication, :Profile, :NCStreaming, :SsoDomains");
                    }
                }
                else {
                    UnknownNode((object)o, @":Login, :ExternalAuthentication, :Profile, :NCStreaming, :SsoDomains");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations11, ref readerCount11);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.ExternalAuthentication Read3_ExternalAuthentication(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id3_ExternalAuthentication && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.ExternalAuthentication o;
            o = new global::Gaffeine.Data.Models.XmlGames.ExternalAuthentication();
            bool[] paramsRead = new bool[2];
            while (Reader.MoveToNextAttribute()) {
                if (!paramsRead[0] && ((object) Reader.LocalName == (object)id57_Facebook && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Facebook = System.Xml.XmlConvert.ToBoolean(Reader.Value);
                    paramsRead[0] = true;
                }
                else if (!paramsRead[1] && ((object) Reader.LocalName == (object)id58_Google && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Google = System.Xml.XmlConvert.ToBoolean(Reader.Value);
                    paramsRead[1] = true;
                }
                else if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o, @":Facebook, :Google");
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations13 = 0;
            int readerCount13 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    UnknownNode((object)o, @"");
                }
                else {
                    UnknownNode((object)o, @"");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations13, ref readerCount13);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.L1UpdaterInfo Read7_L1UpdaterInfo(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id6_L1UpdaterInfo && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.L1UpdaterInfo o;
            o = new global::Gaffeine.Data.Models.XmlGames.L1UpdaterInfo();
            bool[] paramsRead = new bool[1];
            while (Reader.MoveToNextAttribute()) {
                if (!paramsRead[0] && ((object) Reader.LocalName == (object)id59_ServerType && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@ServerType = Read6_L1UpdateServerNetwork(Reader.Value);
                    paramsRead[0] = true;
                }
                else if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o, @":ServerType");
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations14 = 0;
            int readerCount14 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    UnknownNode((object)o, @"");
                }
                else {
                    UnknownNode((object)o, @"");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations14, ref readerCount14);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Enums.L1UpdateServerNetwork Read6_L1UpdateServerNetwork(string s) {
            switch (s) {
                case @"DEV_ALPHA": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@DEV_ALPHA;
                case @"DEV_BETA": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@DEV_BETA;
                case @"SANDBOX_BETA": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@SANDBOX_BETA;
                case @"RC": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@RC;
                case @"RC_TEST": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@RC_TEST;
                case @"CLONE": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@CLONE;
                case @"TEST": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@TEST;
                case @"LIVE_KOREA": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@LIVE_KOREA;
                case @"LIVE_JAPAN": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@LIVE_JAPAN;
                case @"LIVE_TAIWAN": return global::Gaffeine.Data.Enums.L1UpdateServerNetwork.@LIVE_TAIWAN;
                default: throw CreateUnknownConstantException(s, typeof(global::Gaffeine.Data.Enums.L1UpdateServerNetwork));
            }
        }

        global::Gaffeine.Data.Models.XmlGames.Games Read16_Games(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id5_Games && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.Games o;
            o = new global::Gaffeine.Data.Models.XmlGames.Games();
            if ((object)(o.@Game) == null) o.@Game = new global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game>();
            global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game> a_1 = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game>)o.@Game;
            bool[] paramsRead = new bool[2];
            while (Reader.MoveToNextAttribute()) {
                if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o);
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations15 = 0;
            int readerCount15 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    if (!paramsRead[0] && ((object) Reader.LocalName == (object)id7_Launcher && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@Launcher = Read15_Launcher(false, true);
                        paramsRead[0] = true;
                    }
                    else if (((object) Reader.LocalName == (object)id4_Game && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        if ((object)(a_1) == null) Reader.Skip(); else a_1.Add(Read9_Game(false, true));
                    }
                    else {
                        UnknownNode((object)o, @":Launcher, :Game");
                    }
                }
                else {
                    UnknownNode((object)o, @":Launcher, :Game");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations15, ref readerCount15);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Models.XmlGames.Game Read9_Game(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id4_Game && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.Game o;
            o = new global::Gaffeine.Data.Models.XmlGames.Game();
            if ((object)(o.@Menu) == null) o.@Menu = new global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.MenuItem>();
            global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.MenuItem> a_10 = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.MenuItem>)o.@Menu;
            if ((object)(o.@TestGame) == null) o.@TestGame = new global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game>();
            global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game> a_11 = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game>)o.@TestGame;
            if ((object)(o.@AlternativeRegistrySubKeys) == null) o.@AlternativeRegistrySubKeys = new global::System.Collections.Generic.List<global::System.String>();
            global::System.Collections.Generic.List<global::System.String> a_19 = (global::System.Collections.Generic.List<global::System.String>)o.@AlternativeRegistrySubKeys;
            if ((object)(o.@PublishContinents) == null) o.@PublishContinents = new global::System.Collections.Generic.List<global::Gaffeine.Data.Enums.PublishContinent>();
            global::System.Collections.Generic.List<global::Gaffeine.Data.Enums.PublishContinent> a_22 = (global::System.Collections.Generic.List<global::Gaffeine.Data.Enums.PublishContinent>)o.@PublishContinents;
            bool[] paramsRead = new bool[24];
            while (Reader.MoveToNextAttribute()) {
                if (!paramsRead[0] && ((object) Reader.LocalName == (object)id60_IsRepresentative && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@IsRepresentative = System.Xml.XmlConvert.ToBoolean(Reader.Value);
                    paramsRead[0] = true;
                }
                else if (!paramsRead[2] && ((object) Reader.LocalName == (object)id29_Title && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@Title = Reader.Value;
                    paramsRead[2] = true;
                }
                else if (!paramsRead[3] && ((object) Reader.LocalName == (object)id61_GameServerName && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@GameServerName = Reader.Value;
                    paramsRead[3] = true;
                }
                else if (!paramsRead[4] && ((object) Reader.LocalName == (object)id40_ResFolder && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@ResourceId = Reader.Value;
                    paramsRead[4] = true;
                }
                else if (!paramsRead[5] && ((object) Reader.LocalName == (object)id41_DownloadFolder && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@DownloadFolder = Reader.Value;
                    paramsRead[5] = true;
                }
                else if (!paramsRead[6] && ((object) Reader.LocalName == (object)id42_GameID && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@GameId = Reader.Value;
                    paramsRead[6] = true;
                }
                else if (!paramsRead[7] && ((object) Reader.LocalName == (object)id43_UpdateSrvAddr && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@UpdateServerAddress = Reader.Value;
                    paramsRead[7] = true;
                }
                else if (!paramsRead[8] && ((object) Reader.LocalName == (object)id62_AppId && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@AppId = Reader.Value;
                    paramsRead[8] = true;
                }
                else if (!paramsRead[9] && ((object) Reader.LocalName == (object)id63_AppGroupId && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@AppGroupId = Reader.Value;
                    paramsRead[9] = true;
                }
                else if (!paramsRead[12] && ((object) Reader.LocalName == (object)id64_AllowMultiClient && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@AllowMultiClient = Reader.Value;
                    paramsRead[12] = true;
                }
                else if (!paramsRead[13] && ((object) Reader.LocalName == (object)id65_AllowOverlayAgent && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@AllowOverlayAgent = Reader.Value;
                    paramsRead[13] = true;
                }
                else if (!paramsRead[14] && ((object) Reader.LocalName == (object)id66_OverlayGameSignature && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@OverlayGameSignature = Reader.Value;
                    paramsRead[14] = true;
                }
                else if (!paramsRead[15] && ((object) Reader.LocalName == (object)id67_CrossPromotionAppGroupCode && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@CrossPromotionAppGroupCode = Reader.Value;
                    paramsRead[15] = true;
                }
                else if (!paramsRead[16] && ((object) Reader.LocalName == (object)id68_CrossPromotionMissionKey && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@CrossPromotionMissionKey = Reader.Value;
                    paramsRead[16] = true;
                }
                else if (!paramsRead[17] && ((object) Reader.LocalName == (object)id69_GameRatingAge && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@GameRatingAge = Reader.Value;
                    paramsRead[17] = true;
                }
                else if (!paramsRead[21] && ((object) Reader.LocalName == (object)id70_NetworkScope && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@NetworkScope = Reader.Value;
                    paramsRead[21] = true;
                }
                else if (!paramsRead[23] && ((object) Reader.LocalName == (object)id38_CreateAccountUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                    o.@CreateAccountUri = Reader.Value;
                    paramsRead[23] = true;
                }
                else if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o, @":IsRepresentative, :Title, :GameServerName, :ResFolder, :DownloadFolder, :GameID, :UpdateSrvAddr, :AppId, :AppGroupId, :AllowMultiClient, :AllowOverlayAgent, :OverlayGameSignature, :CrossPromotionAppGroupCode, :CrossPromotionMissionKey, :GameRatingAge, :NetworkScope, :CreateAccountUri");
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations16 = 0;
            int readerCount16 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    if (!paramsRead[1] && ((object) Reader.LocalName == (object)id15_WebContentFilters && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@WebContentFilters = Read4_WebContentFilters(false, true);
                        paramsRead[1] = true;
                    }
                    else if (((object) Reader.LocalName == (object)id71_Menu && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        if (!ReadNull()) {
                            if ((object)(o.@Menu) == null) o.@Menu = new global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.MenuItem>();
                            global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.MenuItem> a_10_0 = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.MenuItem>)o.@Menu;
                            if ((Reader.IsEmptyElement)) {
                                Reader.Skip();
                            }
                            else {
                                Reader.ReadStartElement();
                                Reader.MoveToContent();
                                int whileIterations17 = 0;
                                int readerCount17 = ReaderCount;
                                while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                                    if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                                        if (((object) Reader.LocalName == (object)id9_MenuItem && (object) Reader.NamespaceURI == (object)id2_Item)) {
                                            if ((object)(a_10_0) == null) Reader.Skip(); else a_10_0.Add(Read5_MenuItem(true, true));
                                        }
                                        else {
                                            UnknownNode(null, @":MenuItem");
                                        }
                                    }
                                    else {
                                        UnknownNode(null, @":MenuItem");
                                    }
                                    Reader.MoveToContent();
                                    CheckReaderCount(ref whileIterations17, ref readerCount17);
                                }
                            ReadEndElement();
                            }
                        }
                    }
                    else if (((object) Reader.LocalName == (object)id72_TestServerGames && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        if (!ReadNull()) {
                            if ((object)(o.@TestGame) == null) o.@TestGame = new global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game>();
                            global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game> a_11_0 = (global::System.Collections.Generic.List<global::Gaffeine.Data.Models.XmlGames.Game>)o.@TestGame;
                            if ((Reader.IsEmptyElement)) {
                                Reader.Skip();
                            }
                            else {
                                Reader.ReadStartElement();
                                Reader.MoveToContent();
                                int whileIterations18 = 0;
                                int readerCount18 = ReaderCount;
                                while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                                    if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                                        if (((object) Reader.LocalName == (object)id73_TestGame && (object) Reader.NamespaceURI == (object)id2_Item)) {
                                            if ((object)(a_11_0) == null) Reader.Skip(); else a_11_0.Add(Read9_Game(true, true));
                                        }
                                        else {
                                            UnknownNode(null, @":TestGame");
                                        }
                                    }
                                    else {
                                        UnknownNode(null, @":TestGame");
                                    }
                                    Reader.MoveToContent();
                                    CheckReaderCount(ref whileIterations18, ref readerCount18);
                                }
                            ReadEndElement();
                            }
                        }
                    }
                    else if (!paramsRead[18] && ((object) Reader.LocalName == (object)id6_L1UpdaterInfo && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@L1UpdaterInfo = Read7_L1UpdaterInfo(false, true);
                        paramsRead[18] = true;
                    }
                    else if (((object) Reader.LocalName == (object)id74_AlternativeRegistrySubKeys && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        if (!ReadNull()) {
                            if ((object)(o.@AlternativeRegistrySubKeys) == null) o.@AlternativeRegistrySubKeys = new global::System.Collections.Generic.List<global::System.String>();
                            global::System.Collections.Generic.List<global::System.String> a_19_0 = (global::System.Collections.Generic.List<global::System.String>)o.@AlternativeRegistrySubKeys;
                            if ((Reader.IsEmptyElement)) {
                                Reader.Skip();
                            }
                            else {
                                Reader.ReadStartElement();
                                Reader.MoveToContent();
                                int whileIterations19 = 0;
                                int readerCount19 = ReaderCount;
                                while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                                    if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                                        if (((object) Reader.LocalName == (object)id75_SubKey && (object) Reader.NamespaceURI == (object)id2_Item)) {
                                            if (ReadNull()) {
                                                a_19_0.Add(null);
                                            }
                                            else {
                                                a_19_0.Add(Reader.ReadElementString());
                                            }
                                        }
                                        else {
                                            UnknownNode(null, @":SubKey");
                                        }
                                    }
                                    else {
                                        UnknownNode(null, @":SubKey");
                                    }
                                    Reader.MoveToContent();
                                    CheckReaderCount(ref whileIterations19, ref readerCount19);
                                }
                            ReadEndElement();
                            }
                        }
                    }
                    else if (!paramsRead[20] && ((object) Reader.LocalName == (object)id1_BusinessModel && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        o.@BusinessModel = Read2_BusinessModel(false, true);
                        paramsRead[20] = true;
                    }
                    else if (((object) Reader.LocalName == (object)id76_PublishContinents && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        if (!ReadNull()) {
                            if ((object)(o.@PublishContinents) == null) o.@PublishContinents = new global::System.Collections.Generic.List<global::Gaffeine.Data.Enums.PublishContinent>();
                            global::System.Collections.Generic.List<global::Gaffeine.Data.Enums.PublishContinent> a_22_0 = (global::System.Collections.Generic.List<global::Gaffeine.Data.Enums.PublishContinent>)o.@PublishContinents;
                            if ((Reader.IsEmptyElement)) {
                                Reader.Skip();
                            }
                            else {
                                Reader.ReadStartElement();
                                Reader.MoveToContent();
                                int whileIterations20 = 0;
                                int readerCount20 = ReaderCount;
                                while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                                    if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                                        if (((object) Reader.LocalName == (object)id77_Continent && (object) Reader.NamespaceURI == (object)id2_Item)) {
                                            {
                                                a_22_0.Add(Read8_PublishContinent(Reader.ReadElementString()));
                                            }
                                        }
                                        else {
                                            UnknownNode(null, @":Continent");
                                        }
                                    }
                                    else {
                                        UnknownNode(null, @":Continent");
                                    }
                                    Reader.MoveToContent();
                                    CheckReaderCount(ref whileIterations20, ref readerCount20);
                                }
                            ReadEndElement();
                            }
                        }
                    }
                    else {
                        UnknownNode((object)o, @":WebContentFilters, :Menu, :TestServerGames, :L1UpdaterInfo, :AlternativeRegistrySubKeys, :BusinessModel, :PublishContinents");
                    }
                }
                else {
                    UnknownNode((object)o, @":WebContentFilters, :Menu, :TestServerGames, :L1UpdaterInfo, :AlternativeRegistrySubKeys, :BusinessModel, :PublishContinents");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations16, ref readerCount16);
            }
            ReadEndElement();
            return o;
        }

        global::Gaffeine.Data.Enums.PublishContinent Read8_PublishContinent(string s) {
            switch (s) {
                case @"NONE": return global::Gaffeine.Data.Enums.PublishContinent.@NONE;
                case @"NA": return global::Gaffeine.Data.Enums.PublishContinent.@NA;
                case @"SA": return global::Gaffeine.Data.Enums.PublishContinent.@SA;
                case @"EU": return global::Gaffeine.Data.Enums.PublishContinent.@EU;
                case @"OC": return global::Gaffeine.Data.Enums.PublishContinent.@OC;
                default: throw CreateUnknownConstantException(s, typeof(global::Gaffeine.Data.Enums.PublishContinent));
            }
        }

        global::Gaffeine.Data.Models.XmlGames.BusinessModel Read2_BusinessModel(bool isNullable, bool checkType) {
            System.Xml.XmlQualifiedName xsiType = checkType ? GetXsiType() : null;
            bool isNull = false;
            if (isNullable) isNull = ReadNull();
            if (checkType) {
            if (xsiType == null || ((object) ((System.Xml.XmlQualifiedName)xsiType).Name == (object)id1_BusinessModel && (object) ((System.Xml.XmlQualifiedName)xsiType).Namespace == (object)id2_Item)) {
            }
            else
                throw CreateUnknownTypeException((System.Xml.XmlQualifiedName)xsiType);
            }
            if (isNull) return null;
            global::Gaffeine.Data.Models.XmlGames.BusinessModel o;
            o = new global::Gaffeine.Data.Models.XmlGames.BusinessModel();
            bool[] paramsRead = new bool[3];
            while (Reader.MoveToNextAttribute()) {
                if (!IsXmlnsAttribute(Reader.Name)) {
                    UnknownNode((object)o);
                }
            }
            Reader.MoveToElement();
            if (Reader.IsEmptyElement) {
                Reader.Skip();
                return o;
            }
            Reader.ReadStartElement();
            Reader.MoveToContent();
            int whileIterations21 = 0;
            int readerCount21 = ReaderCount;
            while (Reader.NodeType != System.Xml.XmlNodeType.EndElement && Reader.NodeType != System.Xml.XmlNodeType.None) {
                if (Reader.NodeType == System.Xml.XmlNodeType.Element) {
                    if (!paramsRead[0] && ((object) Reader.LocalName == (object)id78_BusinessModelKind && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        {
                            o.@BusinessModelKind = Reader.ReadElementString();
                        }
                        paramsRead[0] = true;
                    }
                    else if (!paramsRead[1] && ((object) Reader.LocalName == (object)id79_AppGroupCode && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        {
                            o.@AppGroupCode = Reader.ReadElementString();
                        }
                        paramsRead[1] = true;
                    }
                    else if (!paramsRead[2] && ((object) Reader.LocalName == (object)id80_PurchaseUri && (object) Reader.NamespaceURI == (object)id2_Item)) {
                        {
                            o.@PurchaseUri = Reader.ReadElementString();
                        }
                        paramsRead[2] = true;
                    }
                    else {
                        UnknownNode((object)o, @":BusinessModelKind, :AppGroupCode, :PurchaseUri");
                    }
                }
                else {
                    UnknownNode((object)o, @":BusinessModelKind, :AppGroupCode, :PurchaseUri");
                }
                Reader.MoveToContent();
                CheckReaderCount(ref whileIterations21, ref readerCount21);
            }
            ReadEndElement();
            return o;
        }

        protected override void InitCallbacks() {
        }

        string id65_AllowOverlayAgent;
        string id48_BuffToonUri;
        string id51_OpenBetaEventUri;
        string id23_Present;
        string id1_BusinessModel;
        string id55_SsoDomains;
        string id28_Center;
        string id25_Content;
        string id59_ServerType;
        string id69_GameRatingAge;
        string id52_TeenServiceGamePlayTimeUri;
        string id46_AccountWebUri;
        string id50_PlayncUri;
        string id78_BusinessModelKind;
        string id45_LoginWebAppId;
        string id39_CantLoginUri;
        string id42_GameID;
        string id62_AppId;
        string id22_Charge;
        string id41_DownloadFolder;
        string id68_CrossPromotionMissionKey;
        string id67_CrossPromotionAppGroupCode;
        string id2_Item;
        string id18_ExcludeInternalBrowsingUris;
        string id36_Image;
        string id37_External_Browser;
        string id19_Uri;
        string id53_DownloadPageUri;
        string id71_Menu;
        string id24_MyPage;
        string id10_NCStreaming;
        string id26_Coupon;
        string id72_TestServerGames;
        string id47_NCComixUri;
        string id5_Games;
        string id73_TestGame;
        string id49_HelpUri;
        string id30_FacebookTag;
        string id38_CreateAccountUri;
        string id54_OverrideCss;
        string id43_UpdateSrvAddr;
        string id4_Game;
        string id3_ExternalAuthentication;
        string id12_Pattern;
        string id75_SubKey;
        string id13_Profile;
        string id74_AlternativeRegistrySubKeys;
        string id31_GameName;
        string id14_ProfileUriInfo;
        string id57_Facebook;
        string id17_UriRegex;
        string id64_AllowMultiClient;
        string id70_NetworkScope;
        string id9_MenuItem;
        string id66_OverlayGameSignature;
        string id32_ExePath;
        string id21_WindowTitleResourceKey;
        string id60_IsRepresentative;
        string id20_IsExternal;
        string id34_Source;
        string id33_Sources;
        string id7_Launcher;
        string id15_WebContentFilters;
        string id77_Continent;
        string id58_Google;
        string id27_Security;
        string id63_AppGroupId;
        string id29_Title;
        string id6_L1UpdaterInfo;
        string id35_Id;
        string id79_AppGroupCode;
        string id56_DomainRegex;
        string id61_GameServerName;
        string id44_LoginWebUri;
        string id16_InternalBrowsingUris;
        string id11_NCStreamingSource;
        string id80_PurchaseUri;
        string id8_Login;
        string id40_ResFolder;
        string id76_PublishContinents;

        protected override void InitIDs() {
            id65_AllowOverlayAgent = Reader.NameTable.Add(@"AllowOverlayAgent");
            id48_BuffToonUri = Reader.NameTable.Add(@"BuffToonUri");
            id51_OpenBetaEventUri = Reader.NameTable.Add(@"OpenBetaEventUri");
            id23_Present = Reader.NameTable.Add(@"Present");
            id1_BusinessModel = Reader.NameTable.Add(@"BusinessModel");
            id55_SsoDomains = Reader.NameTable.Add(@"SsoDomains");
            id28_Center = Reader.NameTable.Add(@"Center");
            id25_Content = Reader.NameTable.Add(@"Content");
            id59_ServerType = Reader.NameTable.Add(@"ServerType");
            id69_GameRatingAge = Reader.NameTable.Add(@"GameRatingAge");
            id52_TeenServiceGamePlayTimeUri = Reader.NameTable.Add(@"TeenServiceGamePlayTimeUri");
            id46_AccountWebUri = Reader.NameTable.Add(@"AccountWebUri");
            id50_PlayncUri = Reader.NameTable.Add(@"PlayncUri");
            id78_BusinessModelKind = Reader.NameTable.Add(@"BusinessModelKind");
            id45_LoginWebAppId = Reader.NameTable.Add(@"LoginWebAppId");
            id39_CantLoginUri = Reader.NameTable.Add(@"CantLoginUri");
            id42_GameID = Reader.NameTable.Add(@"GameID");
            id62_AppId = Reader.NameTable.Add(@"AppId");
            id22_Charge = Reader.NameTable.Add(@"Charge");
            id41_DownloadFolder = Reader.NameTable.Add(@"DownloadFolder");
            id68_CrossPromotionMissionKey = Reader.NameTable.Add(@"CrossPromotionMissionKey");
            id67_CrossPromotionAppGroupCode = Reader.NameTable.Add(@"CrossPromotionAppGroupCode");
            id2_Item = Reader.NameTable.Add(@"");
            id18_ExcludeInternalBrowsingUris = Reader.NameTable.Add(@"ExcludeInternalBrowsingUris");
            id36_Image = Reader.NameTable.Add(@"Image");
            id37_External_Browser = Reader.NameTable.Add(@"External_Browser");
            id19_Uri = Reader.NameTable.Add(@"Uri");
            id53_DownloadPageUri = Reader.NameTable.Add(@"DownloadPageUri");
            id71_Menu = Reader.NameTable.Add(@"Menu");
            id24_MyPage = Reader.NameTable.Add(@"MyPage");
            id10_NCStreaming = Reader.NameTable.Add(@"NCStreaming");
            id26_Coupon = Reader.NameTable.Add(@"Coupon");
            id72_TestServerGames = Reader.NameTable.Add(@"TestServerGames");
            id47_NCComixUri = Reader.NameTable.Add(@"NCComixUri");
            id5_Games = Reader.NameTable.Add(@"Games");
            id73_TestGame = Reader.NameTable.Add(@"TestGame");
            id49_HelpUri = Reader.NameTable.Add(@"HelpUri");
            id30_FacebookTag = Reader.NameTable.Add(@"FacebookTag");
            id38_CreateAccountUri = Reader.NameTable.Add(@"CreateAccountUri");
            id54_OverrideCss = Reader.NameTable.Add(@"OverrideCss");
            id43_UpdateSrvAddr = Reader.NameTable.Add(@"UpdateSrvAddr");
            id4_Game = Reader.NameTable.Add(@"Game");
            id3_ExternalAuthentication = Reader.NameTable.Add(@"ExternalAuthentication");
            id12_Pattern = Reader.NameTable.Add(@"Pattern");
            id75_SubKey = Reader.NameTable.Add(@"SubKey");
            id13_Profile = Reader.NameTable.Add(@"Profile");
            id74_AlternativeRegistrySubKeys = Reader.NameTable.Add(@"AlternativeRegistrySubKeys");
            id31_GameName = Reader.NameTable.Add(@"GameName");
            id14_ProfileUriInfo = Reader.NameTable.Add(@"ProfileUriInfo");
            id57_Facebook = Reader.NameTable.Add(@"Facebook");
            id17_UriRegex = Reader.NameTable.Add(@"UriRegex");
            id64_AllowMultiClient = Reader.NameTable.Add(@"AllowMultiClient");
            id70_NetworkScope = Reader.NameTable.Add(@"NetworkScope");
            id9_MenuItem = Reader.NameTable.Add(@"MenuItem");
            id66_OverlayGameSignature = Reader.NameTable.Add(@"OverlayGameSignature");
            id32_ExePath = Reader.NameTable.Add(@"ExePath");
            id21_WindowTitleResourceKey = Reader.NameTable.Add(@"WindowTitleResourceKey");
            id60_IsRepresentative = Reader.NameTable.Add(@"IsRepresentative");
            id20_IsExternal = Reader.NameTable.Add(@"IsExternal");
            id34_Source = Reader.NameTable.Add(@"Source");
            id33_Sources = Reader.NameTable.Add(@"Sources");
            id7_Launcher = Reader.NameTable.Add(@"Launcher");
            id15_WebContentFilters = Reader.NameTable.Add(@"WebContentFilters");
            id77_Continent = Reader.NameTable.Add(@"Continent");
            id58_Google = Reader.NameTable.Add(@"Google");
            id27_Security = Reader.NameTable.Add(@"Security");
            id63_AppGroupId = Reader.NameTable.Add(@"AppGroupId");
            id29_Title = Reader.NameTable.Add(@"Title");
            id6_L1UpdaterInfo = Reader.NameTable.Add(@"L1UpdaterInfo");
            id35_Id = Reader.NameTable.Add(@"Id");
            id79_AppGroupCode = Reader.NameTable.Add(@"AppGroupCode");
            id56_DomainRegex = Reader.NameTable.Add(@"DomainRegex");
            id61_GameServerName = Reader.NameTable.Add(@"GameServerName");
            id44_LoginWebUri = Reader.NameTable.Add(@"LoginWebUri");
            id16_InternalBrowsingUris = Reader.NameTable.Add(@"InternalBrowsingUris");
            id11_NCStreamingSource = Reader.NameTable.Add(@"NCStreamingSource");
            id80_PurchaseUri = Reader.NameTable.Add(@"PurchaseUri");
            id8_Login = Reader.NameTable.Add(@"Login");
            id40_ResFolder = Reader.NameTable.Add(@"ResFolder");
            id76_PublishContinents = Reader.NameTable.Add(@"PublishContinents");
        }
    }

    public abstract class XmlSerializer1 : System.Xml.Serialization.XmlSerializer {
        protected override System.Xml.Serialization.XmlSerializationReader CreateReader() {
            return new XmlSerializationReader1();
        }
        protected override System.Xml.Serialization.XmlSerializationWriter CreateWriter() {
            return new XmlSerializationWriter1();
        }
    }

    public sealed class BusinessModelSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"BusinessModel", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write18_BusinessModel(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read18_BusinessModel();
        }
    }

    public sealed class ExternalAuthenticationSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"ExternalAuthentication", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write19_ExternalAuthentication(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read19_ExternalAuthentication();
        }
    }

    public sealed class GameSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"Game", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write20_Game(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read20_Game();
        }
    }

    public sealed class GamesSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"Games", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write21_Games(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read21_Games();
        }
    }

    public sealed class L1UpdaterInfoSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"L1UpdaterInfo", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write22_L1UpdaterInfo(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read22_L1UpdaterInfo();
        }
    }

    public sealed class LauncherSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"Launcher", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write23_Launcher(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read23_Launcher();
        }
    }

    public sealed class LoginSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"Login", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write24_Login(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read24_Login();
        }
    }

    public sealed class MenuItemSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"MenuItem", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write25_MenuItem(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read25_MenuItem();
        }
    }

    public sealed class NCStreamingSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"NCStreaming", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write26_NCStreaming(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read26_NCStreaming();
        }
    }

    public sealed class NCStreamingSourceSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"NCStreamingSource", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write27_NCStreamingSource(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read27_NCStreamingSource();
        }
    }

    public sealed class PatternSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"Pattern", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write28_Pattern(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read28_Pattern();
        }
    }

    public sealed class ProfileSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"Profile", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write29_Profile(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read29_Profile();
        }
    }

    public sealed class ProfileUriInfoSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"ProfileUriInfo", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write30_ProfileUriInfo(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read30_ProfileUriInfo();
        }
    }

    public sealed class WebContentFiltersSerializer : XmlSerializer1 {

        public override System.Boolean CanDeserialize(System.Xml.XmlReader xmlReader) {
            return xmlReader.IsStartElement(@"WebContentFilters", @"");
        }

        protected override void Serialize(object objectToSerialize, System.Xml.Serialization.XmlSerializationWriter writer) {
            ((XmlSerializationWriter1)writer).Write31_WebContentFilters(objectToSerialize);
        }

        protected override object Deserialize(System.Xml.Serialization.XmlSerializationReader reader) {
            return ((XmlSerializationReader1)reader).Read31_WebContentFilters();
        }
    }

    public partial class XmlSerializerContract : global::System.Xml.Serialization.XmlSerializerImplementation {
        public override global::System.Xml.Serialization.XmlSerializationReader Reader { get { return new XmlSerializationReader1(); } }
        public override global::System.Xml.Serialization.XmlSerializationWriter Writer { get { return new XmlSerializationWriter1(); } }
        System.Collections.Hashtable readMethods = null;
        public override System.Collections.Hashtable ReadMethods {
            get {
                if (readMethods == null) {
                    System.Collections.Hashtable _tmp = new System.Collections.Hashtable();
                    _tmp[@"Gaffeine.Data.Models.XmlGames.BusinessModel::"] = @"Read18_BusinessModel";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.ExternalAuthentication::"] = @"Read19_ExternalAuthentication";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Game::"] = @"Read20_Game";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Games::Games:True:"] = @"Read21_Games";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.L1UpdaterInfo::"] = @"Read22_L1UpdaterInfo";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Launcher::"] = @"Read23_Launcher";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Login::"] = @"Read24_Login";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.MenuItem::"] = @"Read25_MenuItem";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.NCStreaming::"] = @"Read26_NCStreaming";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.NCStreamingSource::"] = @"Read27_NCStreamingSource";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Pattern::"] = @"Read28_Pattern";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Profile::"] = @"Read29_Profile";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.ProfileUriInfo::"] = @"Read30_ProfileUriInfo";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.WebContentFilters::"] = @"Read31_WebContentFilters";
                    if (readMethods == null) readMethods = _tmp;
                }
                return readMethods;
            }
        }
        System.Collections.Hashtable writeMethods = null;
        public override System.Collections.Hashtable WriteMethods {
            get {
                if (writeMethods == null) {
                    System.Collections.Hashtable _tmp = new System.Collections.Hashtable();
                    _tmp[@"Gaffeine.Data.Models.XmlGames.BusinessModel::"] = @"Write18_BusinessModel";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.ExternalAuthentication::"] = @"Write19_ExternalAuthentication";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Game::"] = @"Write20_Game";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Games::Games:True:"] = @"Write21_Games";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.L1UpdaterInfo::"] = @"Write22_L1UpdaterInfo";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Launcher::"] = @"Write23_Launcher";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Login::"] = @"Write24_Login";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.MenuItem::"] = @"Write25_MenuItem";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.NCStreaming::"] = @"Write26_NCStreaming";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.NCStreamingSource::"] = @"Write27_NCStreamingSource";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Pattern::"] = @"Write28_Pattern";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.Profile::"] = @"Write29_Profile";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.ProfileUriInfo::"] = @"Write30_ProfileUriInfo";
                    _tmp[@"Gaffeine.Data.Models.XmlGames.WebContentFilters::"] = @"Write31_WebContentFilters";
                    if (writeMethods == null) writeMethods = _tmp;
                }
                return writeMethods;
            }
        }
        System.Collections.Hashtable typedSerializers = null;
        public override System.Collections.Hashtable TypedSerializers {
            get {
                if (typedSerializers == null) {
                    System.Collections.Hashtable _tmp = new System.Collections.Hashtable();
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.ExternalAuthentication::", new ExternalAuthenticationSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.Game::", new GameSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.Games::Games:True:", new GamesSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.BusinessModel::", new BusinessModelSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.Launcher::", new LauncherSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.ProfileUriInfo::", new ProfileUriInfoSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.Login::", new LoginSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.Pattern::", new PatternSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.WebContentFilters::", new WebContentFiltersSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.MenuItem::", new MenuItemSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.Profile::", new ProfileSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.NCStreamingSource::", new NCStreamingSourceSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.L1UpdaterInfo::", new L1UpdaterInfoSerializer());
                    _tmp.Add(@"Gaffeine.Data.Models.XmlGames.NCStreaming::", new NCStreamingSerializer());
                    if (typedSerializers == null) typedSerializers = _tmp;
                }
                return typedSerializers;
            }
        }
        public override System.Boolean CanSerialize(System.Type type) {
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.BusinessModel)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.ExternalAuthentication)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Game)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Games)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.L1UpdaterInfo)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Launcher)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Login)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.MenuItem)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.NCStreaming)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.NCStreamingSource)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Pattern)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Profile)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)) return true;
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.WebContentFilters)) return true;
            return false;
        }
        public override System.Xml.Serialization.XmlSerializer GetSerializer(System.Type type) {
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.BusinessModel)) return new BusinessModelSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.ExternalAuthentication)) return new ExternalAuthenticationSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Game)) return new GameSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Games)) return new GamesSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.L1UpdaterInfo)) return new L1UpdaterInfoSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Launcher)) return new LauncherSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Login)) return new LoginSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.MenuItem)) return new MenuItemSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.NCStreaming)) return new NCStreamingSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.NCStreamingSource)) return new NCStreamingSourceSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Pattern)) return new PatternSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.Profile)) return new ProfileSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.ProfileUriInfo)) return new ProfileUriInfoSerializer();
            if (type == typeof(global::Gaffeine.Data.Models.XmlGames.WebContentFilters)) return new WebContentFiltersSerializer();
            return null;
        }
    }
}
