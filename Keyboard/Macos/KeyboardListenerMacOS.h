#ifndef NSAPPLICATION_NSKEYBOARD_CKEYBOARDLISTENERMACOS_H
#define NSAPPLICATION_NSKEYBOARD_CKEYBOARDLISTENERMACOS_H

#include "MacOSKeyboardAPI.h"
#include "Keyboard/AnyKeyboardKiller.h"
#include "KeyTextMaker.h"
#include "ShifterInfo.h"

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

#include <QObject>

#include <future>

namespace NSApplication {
namespace NSKeyboard {

struct CKeyPressing;
struct CKeyReleasing;

class CKeyboardHandler;

namespace NSMacOS {

class CKeyboardListenerMacOSImpl : public QObject {
  Q_OBJECT

  friend class CKiller;

  using CVKCode = CMacOSKeyboardAPI::CVKCode;
  using CEventTapUPtr = CMacOSKeyboardAPI::CEventTapUPtr;
  using CMessagePortUPtr = CMacOSKeyboardAPI::CMessagePortUPtr;

public:
  using CAnyKillerPromise = std::promise<CAnyKeyboardKiller>;
  CKeyboardListenerMacOSImpl(CAnyKillerPromise, CKeyboardHandler*);
  ~CKeyboardListenerMacOSImpl();

signals:
  void KeyPressing(const CKeyPressing&);
  void KeyReleasing(const CKeyReleasing&);

public:
  int exec();

private:
  CFMachPortRef createEventTap();
  void addEventTapToRunLoop();
  static CGEventRef callbackEventTap(CGEventTapProxy, CGEventType, CGEventRef, void*);

  CFMessagePortRef createMessagePort();
  void addMessagePortToRunLoop();
  static CFDataRef callbackMessagePort(CFMessagePortRef, SInt32, CFDataRef, void*);

  static QChar getKeyLabel(CVKCode);
  static CKeyboardListenerMacOSImpl* getListener(void*);

  CEventTapUPtr EventTap_;
  CMessagePortUPtr MessagePort_;
  CShifterInfo ShifterInfo_;
  CKeyTextMaker KeyTextMaker_;
};

// The object provides a way to shut down the listener
class CKiller {
public:
  CKiller() = default;
  CKiller(CFMessagePortRef);

  void stopListener() const;

private:
  CFMessagePortRef KillerPort_;
};

} // namespace NSMacOS
} // namespace NSKeyboard
} // namespace NSApplication

#endif // NSAPPLICATION_NSKEYBOARD_CKEYBOARDLISTENERMACOS_H
