
//  Calculatrice Rose — Interface Graphique Native NKGui (Nkentseu Framework)

#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKEvent/NkMouseEvent.h"
#include "NKCanvas/Core/NkContextDesc.h"
#include "NKCanvas/Core/NkGraphicsApi.h"
#include "NKCanvas/Renderer/Targets/NkRenderWindow.h"
#include "NKCanvas/UI/NkGuiCanvasBackend.h"
#include "NKTime/NkClock.h"
#include "NKMemory/NkUniquePtr.h"
#include "NKGui/NKGui.h"

#include "Calculatrice.hpp"

#include <string>
#include <cstdio>
#include <cmath>

using namespace nkentseu;
using namespace nkentseu::nkgui;
using namespace nkentseu::renderer;

// données de l'application
NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "CalculatriceRose";
    d.appVersion = "1.0.0";
    return d;
})());


//Logique de la Calculatrice

struct EtatCalculatrice {
    Calculatrice   calc;
    std::string    affichage          = "0";
    double         valeurStockee      = 0.0;
    char           operationEnAttente = 0; // '+', '-', '*', '/', '^'
    bool           nouvelleSaisie     = true;
    bool           afficherHistorique = true;
};

static double lireAffichage(const std::string& s) {
    try { return std::stod(s); } catch (...) { return 0.0; }
}

static std::string formaterResultat(double v) {
    if (std::isnan(v)) return "Erreur";
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.10g", v);
    return std::string(buf);
}

static void saisirChiffre(EtatCalculatrice& e, const char* chiffre) {
    if (e.nouvelleSaisie || e.affichage == "0") {
        e.affichage = chiffre;
        e.nouvelleSaisie = false;
    } else {
        e.affichage += chiffre;
    }
}

static void saisirPoint(EtatCalculatrice& e) {
    if (e.nouvelleSaisie) {
        e.affichage = "0.";
        e.nouvelleSaisie = false;
        return;
    }
    if (e.affichage.find('.') == std::string::npos) {
        e.affichage += ".";
    }
}

static void appliquerOperationEnAttente(EtatCalculatrice& e) {
    if (e.operationEnAttente == 0) {
        e.valeurStockee = lireAffichage(e.affichage);
        return;
    }
    double a = e.valeurStockee;
    double b = lireAffichage(e.affichage);
    double resultat = 0.0;

    switch (e.operationEnAttente) {
        case '+': resultat = e.calc.addition(a, b);       break;
        case '-': resultat = e.calc.soustraction(a, b);   break;
        case '*': resultat = e.calc.multiplication(a, b); break;
        case '/': resultat = e.calc.division(a, b);       break;
        case '^': resultat = e.calc.puissance(a, b);      break;
        default:  resultat = b;                           break;
    }
    e.valeurStockee = resultat;
    e.affichage = formaterResultat(resultat);
}

static void choisirOperation(EtatCalculatrice& e, char op) {
    if (!e.nouvelleSaisie) {
        appliquerOperationEnAttente(e);
    }
    e.operationEnAttente = op;
    e.nouvelleSaisie = true;
}

static void calculerEgal(EtatCalculatrice& e) {
    appliquerOperationEnAttente(e);
    e.operationEnAttente = 0;
    e.nouvelleSaisie = true;
}

static void fonctionUnaire(EtatCalculatrice& e, char fct) {
    double a = lireAffichage(e.affichage);
    double resultat = 0.0;
    switch (fct) {
        case 'r': resultat = e.calc.racine(a);        break;
        case 'l': resultat = e.calc.logarithme(a);    break;
        case 'e': resultat = e.calc.exponentielle(a); break;
        case 's': resultat = e.calc.sinus(a);         break;
        case 'c': resultat = e.calc.cosinus(a);       break;
        default: break;
    }
    e.affichage = formaterResultat(resultat);
    e.valeurStockee = resultat;
    e.nouvelleSaisie = true;
}

static void effacerTout(EtatCalculatrice& e) {
    e.affichage = "0";
    e.valeurStockee = 0.0;
    e.operationEnAttente = 0;
    e.nouvelleSaisie = true;
}

static void effacerDernier(EtatCalculatrice& e) {
    if (e.nouvelleSaisie) return;
    if (e.affichage.size() <= 1) {
        e.affichage = "0";
        e.nouvelleSaisie = true;
    } else {
        e.affichage.pop_back();
    }
}

static void changerSigne(EtatCalculatrice& e) {
    double v = lireAffichage(e.affichage);
    e.affichage = formaterResultat(-v);
}


// Point d'entrée NKMain

int nkmain(const NkEntryState& state) {
    (void)state;

    // 1. Fenêtre Windows
    NkWindow window;
    NkWindowConfig cfg;
    cfg.title    = "Calculatrice Rose — Nkentseu NKGui";
    cfg.width    = 460;
    cfg.height   = 720;
    cfg.centered = true;
    cfg.resizable = true;
    if (!window.Create(cfg)) {
        std::printf("Erreur : Impossible de créer la fenêtre.\n");
        return -1;
    }

    // 2. Renderer GPU (DX11 sur Windows, OpenGL sinon)
    NkContextDesc desc;
    desc.api = NkGraphicsApi::NK_GFX_API_AUTO;
    if (desc.api == NkGraphicsApi::NK_GFX_API_AUTO) {
#if defined(NKENTSEU_PLATFORM_WINDOWS)
        desc.api = NkGraphicsApi::NK_GFX_API_DX11;
#else
        desc.api = NkGraphicsApi::NK_GFX_API_OPENGL;
#endif
    }

    auto target = memory::NkMakeUnique<NkRenderWindow>(window, desc);
    if (!target || !target->IsValid()) {
        std::printf("Erreur : Target GPU invalide.\n");
        return -1;
    }

    // 3. NKGui Context & Backend
    auto ctxPtr = memory::NkMakeUnique<NkGuiContext>();
    if (!ctxPtr) return -1;
    NkGuiContext& ctx = *ctxPtr;
    ctx.Init(static_cast<int32>(cfg.width), static_cast<int32>(cfg.height));
    SetCurrentContext(&ctx);

    renderer::NkGuiCanvasBackend backend;
    if (!backend.Init(target->GetRenderer())) {
        std::printf("Erreur : Backend NKGui invalide.\n");
        return -1;
    }

    // 4. Chargement de la Police
    auto fontPtr = memory::NkMakeUnique<NkGuiFont>();
    if (!fontPtr->LoadEmbedded(NkEmbeddedFontId::DroidSans, 20.f)) {
        fontPtr->LoadEmbedded(NkEmbeddedFontId::ProggyClean, 16.f);
    }
    ctx.font = fontPtr.Get();
    if (fontPtr->Valid()) {
        backend.UploadFontGray8(fontPtr->TexId(), fontPtr->pixels, fontPtr->atlasW, fontPtr->atlasH);
    }

    // 5. Thème "Rose" NKGui
    ctx.theme.bgPrimary = NkColor{ 255, 235, 242, 255 }; // Rose clair
    ctx.theme.panel     = NkColor{ 255, 220, 232, 255 }; // Rose panel
    ctx.theme.header    = NkColor{ 240, 120, 165, 255 }; // Rose header
    ctx.theme.accent    = NkColor{ 220, 80, 135, 255 };  // Rose accent
    ctx.theme.text      = NkColor{ 90, 20, 50, 255 };    // Texte foncé

    EtatCalculatrice etat;
    NkClock clock;
    bool running = true;

    // Callbacks d'événements
    auto& events = NkEvents();
    events.AddEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent*) { running = false; });
    events.AddEventCallback<NkMouseMoveEvent>([&](NkMouseMoveEvent* e) {
        ctx.input.mousePos = { static_cast<float32>(e->GetX()), static_cast<float32>(e->GetY()) };
    });
    events.AddEventCallback<NkMouseButtonPressEvent>([&](NkMouseButtonPressEvent* e) {
        if (e->GetButton() == NkMouseButton::NK_MB_LEFT) ctx.input.mouseDown[0] = true;
    });
    events.AddEventCallback<NkMouseButtonReleaseEvent>([&](NkMouseButtonReleaseEvent* e) {
        if (e->GetButton() == NkMouseButton::NK_MB_LEFT) ctx.input.mouseDown[0] = false;
    });

    uint32 lastW = 0, lastH = 0;

    // 6. Boucle de Rendu
    while (running && window.IsOpen()) {
        float32 dt = clock.Tick().delta;
        if (dt <= 0.f) dt = 1.f / 60.f;

        while (NkEvent* ev = NkEvents().PollEvent()) { (void)ev; }
        if (!running) break;

        const math::NkVec2u wsz = target->GetWindow().GetSize();
        if (wsz.x > 0 && wsz.y > 0 && (wsz.x != lastW || wsz.y != lastH)) {
            target->OnResize(wsz.x, wsz.y);
            lastW = wsz.x;
            lastH = wsz.y;
        }
        const math::NkVec2u sz = target->GetSize();
        if (sz.x > 0 && sz.y > 0) {
            ctx.viewW = static_cast<int32>(sz.x);
            ctx.viewH = static_cast<int32>(sz.y);
        }

        ctx.BeginFrame(dt);
        const float32 W = static_cast<float32>(ctx.viewW);
        const float32 H = static_cast<float32>(ctx.viewH);

        // Arrière-plan & En-tête
        ctx.DL().AddRectFilled({ 0.f, 0.f, W, H }, ctx.theme.bgPrimary);
        ctx.DL().AddRectFilled({ 0.f, 0.f, W, 44.f }, ctx.theme.header);
        TextAt(ctx, { 16.f, 12.f }, "Calculatrice Rose — NKGui");

        // Panneau Principal
        const NkRect mainArea = { 12.f, 54.f, W - 24.f, H - 66.f };
        if (BeginPanel(ctx, "Calculatrice", mainArea)) {
            
            // Écran d'Affichage LCD
            Text(ctx, "Affichage :");
            char dispBuf[128];
            if (etat.operationEnAttente != 0) {
                std::snprintf(dispBuf, sizeof(dispBuf), "[%.6g %c]  %s", etat.valeurStockee, etat.operationEnAttente, etat.affichage.c_str());
            } else {
                std::snprintf(dispBuf, sizeof(dispBuf), "%s", etat.affichage.c_str());
            }
            Text(ctx, dispBuf);
            Separator(ctx);

            // Fonctions Scientifiques
            Text(ctx, "Fonctions Scientifiques :");
            if (Button(ctx, "sin"))  fonctionUnaire(etat, 's'); ctx.SameLine();
            if (Button(ctx, "cos"))  fonctionUnaire(etat, 'c'); ctx.SameLine();
            if (Button(ctx, "ln"))   fonctionUnaire(etat, 'l'); ctx.SameLine();
            if (Button(ctx, "exp"))  fonctionUnaire(etat, 'e'); ctx.SameLine();
            if (Button(ctx, "sqrt")) fonctionUnaire(etat, 'r');

            Separator(ctx);

            // Commandes & Opérateurs
            Text(ctx, "Commandes :");
            if (Button(ctx, " C "))   effacerTout(etat);      ctx.SameLine();
            if (Button(ctx, " +/- ")) changerSigne(etat);     ctx.SameLine();
            if (Button(ctx, " ^ "))   choisirOperation(etat, '^'); ctx.SameLine();
            if (Button(ctx, " <- "))  effacerDernier(etat);   ctx.SameLine();
            if (Button(ctx, " / "))   choisirOperation(etat, '/');

            Separator(ctx);

            // Pavé Numérique & Calcul
            Text(ctx, "Pave Numerique :");
            
            // Ligne 7 8 9 *
            if (Button(ctx, " 7 ")) saisirChiffre(etat, "7"); ctx.SameLine();
            if (Button(ctx, " 8 ")) saisirChiffre(etat, "8"); ctx.SameLine();
            if (Button(ctx, " 9 ")) saisirChiffre(etat, "9"); ctx.SameLine();
            if (Button(ctx, " * ")) choisirOperation(etat, '*');

            // Ligne 4 5 6 -
            if (Button(ctx, " 4 ")) saisirChiffre(etat, "4"); ctx.SameLine();
            if (Button(ctx, " 5 ")) saisirChiffre(etat, "5"); ctx.SameLine();
            if (Button(ctx, " 6 ")) saisirChiffre(etat, "6"); ctx.SameLine();
            if (Button(ctx, " - ")) choisirOperation(etat, '-');

            // Ligne 1 2 3 +
            if (Button(ctx, " 1 ")) saisirChiffre(etat, "1"); ctx.SameLine();
            if (Button(ctx, " 2 ")) saisirChiffre(etat, "2"); ctx.SameLine();
            if (Button(ctx, " 3 ")) saisirChiffre(etat, "3"); ctx.SameLine();
            if (Button(ctx, " + ")) choisirOperation(etat, '+');

            // Ligne 0 . =
            if (Button(ctx, " 0 ")) saisirChiffre(etat, "0"); ctx.SameLine();
            if (Button(ctx, " . ")) saisirPoint(etat);       ctx.SameLine();
            if (Button(ctx, "  =  ")) calculerEgal(etat);

            Separator(ctx);

            // Historique
            Checkbox(ctx, "Afficher l'historique", etat.afficherHistorique);
            if (etat.afficherHistorique) {
                if (Button(ctx, "Sauvegarder l'historique")) {
                    etat.calc.sauvegarderHistorique();
                }
                ctx.SameLine();
                if (Button(ctx, "Vider l'historique")) {
                    etat.calc.effacerHistorique();
                }

                const auto& entrees = etat.calc.obtenirHistorique().obtenirEntrees();
                if (entrees.empty()) {
                    Text(ctx, " (Historique vide)");
                } else {
                    int startIdx = (int)entrees.size() - 5;
                    if (startIdx < 0) startIdx = 0;
                    for (size_t i = startIdx; i < entrees.size(); ++i) {
                        Text(ctx, entrees[i].c_str());
                    }
                }
            }

            EndPanel(ctx);
        }

        ctx.EndFrame();

        // Rendu final
        target->Clear(ctx.theme.bgPrimary);
        const math::NkVec2u rsz = target->GetSize();
        backend.Submit(ctx.DL(), rsz.x, rsz.y);
        target->Display();
    }

    return 0;
}
